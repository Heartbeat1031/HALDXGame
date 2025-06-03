// Jolt Physics Library (https://github.com/jrouwe/JoltPhysics)
// SPDX-FileCopyrightText: 2025 Jorrit Rouwe
// SPDX-License-Identifier: CC0-1.0
// This file is in the public domain. It serves as an example to start building your own application using Jolt Physics. Feel free to copy paste without attribution!

// The Jolt headers don't include Jolt.h. Always include Jolt.h before including any other Jolt header.
// You can use Jolt.h in your precompiled header to speed up compilation.
#include <Jolt/Jolt.h>
// Jolt includes
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

// STL includes
#include <iostream>
#include <cstdarg>
#include <thread>

// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
JPH_SUPPRESS_WARNINGS;

// All Jolt symbols are in the JPH namespace
using namespace JPH;

// If you want your code to compile using single or double precision write 0.0_r to get a Real value that compiles to double or float depending if JPH_DOUBLE_PRECISION is set or not.
using namespace JPH::literals;

// We're also using STL classes in this example
using namespace std;

// Callback for traces, connect this to your own trace function if you have one
static void TraceImpl(const char *inFMT, ...)
{
	// Format the message
	va_list list;
	va_start(list, inFMT);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), inFMT, list);
	va_end(list);

	// Print to the TTY
	cout << buffer << endl;
}

#ifdef JPH_ENABLE_ASSERTS

// Callback for asserts, connect this to your own assert handler if you have one
static bool AssertFailedImpl(const char *inExpression, const char *inMessage, const char *inFile, uint inLine)
{
	// Print to the TTY
	cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr? inMessage : "") << endl;

	// Breakpoint
	return true;
};

#endif // JPH_ENABLE_ASSERTS

// Layer that objects can be in, determines which other objects it can collide with
// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
// but only if you do collision testing).
namespace Layers
{
	static constexpr ObjectLayer NON_MOVING = 0;
	static constexpr ObjectLayer MOVING = 1;
	static constexpr ObjectLayer NUM_LAYERS = 2;
};

/// Class that determines if two object layers can collide
class ObjectLayerPairFilterImpl : public ObjectLayerPairFilter
{
public:
	virtual bool					ShouldCollide(ObjectLayer inObject1, ObjectLayer inObject2) const override
	{
		switch (inObject1)
		{
		case Layers::NON_MOVING:
			return inObject2 == Layers::MOVING; // Non moving only collides with moving
		case Layers::MOVING:
			return true; // Moving collides with everything
		default:
			JPH_ASSERT(false);
			return false;
		}
	}
};

// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
namespace BroadPhaseLayers
{
	static constexpr BroadPhaseLayer NON_MOVING(0);
	static constexpr BroadPhaseLayer MOVING(1);
	static constexpr uint NUM_LAYERS(2);
};

// BroadPhaseLayerInterface implementation
// This defines a mapping between object and broadphase layers.
class BPLayerInterfaceImpl final : public BroadPhaseLayerInterface
{
public:
									BPLayerInterfaceImpl()
	{
		// Create a mapping table from object to broad phase layer
		mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
		mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
	}

	virtual uint					GetNumBroadPhaseLayers() const override
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}

	virtual BroadPhaseLayer			GetBroadPhaseLayer(ObjectLayer inLayer) const override
	{
		JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
		return mObjectToBroadPhase[inLayer];
	}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	virtual const char *			GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override
	{
		switch ((BroadPhaseLayer::Type)inLayer)
		{
		case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
		case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
		default:													JPH_ASSERT(false); return "INVALID";
		}
	}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
	BroadPhaseLayer					mObjectToBroadPhase[Layers::NUM_LAYERS];
};

/// Class that determines if an object layer can collide with a broadphase layer
class ObjectVsBroadPhaseLayerFilterImpl : public ObjectVsBroadPhaseLayerFilter
{
public:
	virtual bool				ShouldCollide(ObjectLayer inLayer1, BroadPhaseLayer inLayer2) const override
	{
		switch (inLayer1)
		{
		case Layers::NON_MOVING:
			return inLayer2 == BroadPhaseLayers::MOVING;
		case Layers::MOVING:
			return true;
		default:
			JPH_ASSERT(false);
			return false;
		}
	}
};

// An example contact listener
class MyContactListener : public ContactListener
{
public:
	// See: ContactListener
	virtual ValidateResult	OnContactValidate(const Body &inBody1, const Body &inBody2, RVec3Arg inBaseOffset, const CollideShapeResult &inCollisionResult) override
	{
		cout << "Contact validate callback" << endl;

		// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
		return ValidateResult::AcceptAllContactsForThisBodyPair;
	}

	virtual void			OnContactAdded(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override
	{
		cout << "A contact was added" << endl;
	}

	virtual void			OnContactPersisted(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override
	{
		cout << "A contact was persisted" << endl;
	}

	virtual void			OnContactRemoved(const SubShapeIDPair &inSubShapePair) override
	{
		cout << "A contact was removed" << endl;
	}
};

// An example activation listener
class MyBodyActivationListener : public BodyActivationListener
{
public:
	virtual void		OnBodyActivated(const BodyID &inBodyID, uint64 inBodyUserData) override
	{
		cout << "A body got activated" << endl;
	}

	virtual void		OnBodyDeactivated(const BodyID &inBodyID, uint64 inBodyUserData) override
	{
		cout << "A body went to sleep" << endl;
	}
};

// 程序入口点
int main(int argc, char** argv)
{
	// 注册默认的内存分配器。在本例中我们使用默认的 malloc/free，如果你想自定义分配器可以重写（见 Memory.h）。
	// 必须在调用任何 Jolt 函数之前注册。
	RegisterDefaultAllocator();

	// 安装日志和断言回调
	Trace = TraceImpl;
	JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl);

	// 创建工厂类，此类用于根据类名或哈希值创建类实例，主要用于反序列化保存的数据。
	// 本例中不会直接使用，但仍需创建。
	Factory::sInstance = new Factory();

	// 注册所有物理类型到工厂中，并将它们的碰撞处理器安装到 CollisionDispatch 中。
	// 如果你有自定义形状类型，通常需要在调用此函数前注册它们的碰撞处理器。
	// 如果你自定义了默认物理材质（PhysicsMaterial::sDefault），需在此函数调用前初始化，否则将由此函数自动创建。
	RegisterTypes();

	// 创建临时内存分配器，用于物理模拟中的临时分配。
	// 此处预分配了 10MB，避免在物理更新中频繁分配内存。
	// 虽然 10MB 对此示例来说太多了，但在实际项目中这是一个常见的值。
	// 如果不想预分配，也可以使用 TempAllocatorMalloc 来使用 malloc/free。
	TempAllocatorImpl temp_allocator(10 * 1024 * 1024);

	// 创建作业系统，用于在多个线程上执行物理任务。
	// 通常你会自己实现 JobSystem 接口，并让 Jolt 在你的作业系统上运行。JobSystemThreadPool 是一个示例实现。
	JobSystemThreadPool job_system(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);

	// 设置最大刚体数量，超过此数量将无法添加刚体。
	// 注意：这个值较小仅用于测试，实际项目中应设为 65536 左右。
	const uint cMaxBodies = 1024;

	// 设置刚体互斥锁数量，保护刚体避免并发访问。设为 0 表示使用默认设置。
	const uint cNumBodyMutexes = 0;

	// 设置最大体对数量（由粗阶段碰撞检测生成），缓冲区过小会影响效率。
	// 注意：这个值较小仅用于测试，实际项目中应设为 65536 左右。
	const uint cMaxBodyPairs = 1024;

	// 设置接触约束的最大数量，超过此数量的碰撞将被忽略，可能导致穿透或穿地。
	// 注意：这个值较小仅用于测试，实际项目中应设为 10240 左右。
	const uint cMaxContactConstraints = 1024;

	// 创建对象层到广相层的映射表
	// 注意：此接口由 PhysicsSystem 持有引用，必须保持其生命周期。
	BPLayerInterfaceImpl broad_phase_layer_interface;

	// 创建对象层与广相层之间的过滤器
	// 注意：此接口由 PhysicsSystem 持有引用，必须保持其生命周期。
	ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter;

	// 创建对象层与对象层之间的过滤器
	// 注意：此接口由 PhysicsSystem 持有引用，必须保持其生命周期。
	ObjectLayerPairFilterImpl object_vs_object_layer_filter;

	// 创建物理系统实例
	PhysicsSystem physics_system;
	physics_system.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);

	// 创建刚体激活监听器，用于监听刚体的激活/休眠状态变化
	// 注意：回调将在作业线程中执行，必须是线程安全的。非必需注册。
	MyBodyActivationListener body_activation_listener;
	physics_system.SetBodyActivationListener(&body_activation_listener);

	// 创建碰撞监听器，用于监听刚体的碰撞开始与结束事件
	// 注意：回调将在作业线程中执行，必须是线程安全的。非必需注册。
	MyContactListener contact_listener;
	physics_system.SetContactListener(&contact_listener);

	// 获取刚体接口，用于操作物理系统中的刚体。
	// 这里使用带锁版本（即使我们只在单线程中操作）。
	BodyInterface &body_interface = physics_system.GetBodyInterface();

	// 创建一个地板刚体，一个大尺寸的盒子
	// 创建用于碰撞体积的设置对象（形状设置）
	// 对于简单形状（如盒子），也可以直接创建 BoxShape。
	BoxShapeSettings floor_shape_settings(Vec3(100.0f, 1.0f, 100.0f));
	floor_shape_settings.SetEmbedded(); // 标记为栈上的引用计数对象，避免引用计数为 0 时被释放

	// 创建形状
	ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
	ShapeRefC floor_shape = floor_shape_result.Get(); // 本例中不期望出错，但可以通过 HasError()/GetError() 检查

	// 创建刚体设置，可设置位置、旋转、材质等
	BodyCreationSettings floor_settings(floor_shape, RVec3(0.0_r, -1.0_r, 0.0_r), Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);

	// 创建地板刚体
	Body *floor = body_interface.CreateBody(floor_settings); // 如果刚体数达到上限，可能返回 nullptr

	// 将地板添加到物理世界中
	body_interface.AddBody(floor->GetID(), EActivation::DontActivate);

	// 创建一个动态刚体，在地板上弹跳
	// 使用简洁方式直接创建并添加到世界中
	BodyCreationSettings sphere_settings(new SphereShape(0.5f), RVec3(0.0_r, 2.0_r, 0.0_r), Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
	BodyID sphere_id = body_interface.CreateAndAddBody(sphere_settings, EActivation::Activate);

	// 设置刚体的初始速度
	// （如果用 CreateBody 创建刚体，可以在加入系统前设置速度）
	body_interface.SetLinearVelocity(sphere_id, Vec3(0.0f, -5.0f, 0.0f));

	// 设置每帧物理时间步，60Hz 是一个合适的频率
	const float cDeltaTime = 1.0f / 60.0f;

	// 可选：优化广相阶段，提高碰撞检测性能（此示例中仅两个刚体，没有意义）
	// 注意：此操作代价较高，不应每帧调用，建议批量插入物体后调用。
	physics_system.OptimizeBroadPhase();

	// 模拟物理世界，直到球体进入休眠状态
	uint step = 0;
	while (body_interface.IsActive(sphere_id))
	{
		++step;

		// 输出球体当前位置与速度
		RVec3 position = body_interface.GetCenterOfMassPosition(sphere_id);
		Vec3 velocity = body_interface.GetLinearVelocity(sphere_id);
		cout << "Step " << step << ": Position = (" << position.GetX() << ", " << position.GetY() << ", " << position.GetZ() << "), Velocity = (" << velocity.GetX() << ", " << velocity.GetY() << ", " << velocity.GetZ() << ")" << endl;

		// 如果时间步大于 1/60 秒，需执行多次碰撞检测以保持稳定
		const int cCollisionSteps = 1;

		// 进行一次物理世界的更新
		physics_system.Update(cDeltaTime, cCollisionSteps, &temp_allocator, &job_system);
	}

	// 从物理系统中移除球体（保留状态，可再次添加）
	body_interface.RemoveBody(sphere_id);

	// 销毁球体（ID 无效）
	body_interface.DestroyBody(sphere_id);

	// 移除并销毁地板
	body_interface.RemoveBody(floor->GetID());
	body_interface.DestroyBody(floor->GetID());

	// 反注册所有类型并清理默认材质
	UnregisterTypes();

	// 销毁工厂
	delete Factory::sInstance;
	Factory::sInstance = nullptr;

	return 0;
}
