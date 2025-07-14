#include	<memory>
#include	<random>
#include	<array>
#include    "main.h"
#include	"system/CDirectInput.h"
#include	"system/CommonTypes.h"
#include	"system/CSprite.h"
#include	"system/Transform.h"
#include	"animationsystem.h"
#include	"enemy.h"
#include	"enemybullet.h"

// �G�f�[�^
static std::array<Enemy, ENEMYMAX>		g_enemies;

// �X�v���C�g
static std::unique_ptr<CSprite> g_spr;			// �X�v���C�g	

// �h���S���摜�̃e�N�X�`�����W��`
static UVQuad DragonPicture[12] = {
	// ��O����
	{	Vector2(0.0f,			0.0f),	Vector2(1.0f / 3.0f,0.0f),	Vector2(0.0f,			1.0f / 4.0f),	Vector2(1.0f / 3.0f,1.0f / 4.0f) },
	{	Vector2(1.0f / 3.0f,	0.0f),	Vector2(2.0f / 3.0f,0.0f),	Vector2(1.0f / 3.0f,	1.0f / 4.0f),	Vector2(2.0f / 3.0f,1.0f / 4.0f) },
	{	Vector2(2.0f / 3.0f,	0.0f),	Vector2(3.0f / 3.0f,0.0f),	Vector2(2.0f / 3.0f,	1.0f / 4.0f),	Vector2(3.0f / 3.0f,1.0f / 4.0f) },

	// ������
	{	Vector2(0.0f,			1.0f / 4.0f),	Vector2(1.0f / 3.0f,1.0f / 4.0f),	Vector2(0.0f,		2.0f / 4.0f),	Vector2(1.0f / 3.0f,2.0f / 4.0f) },
	{	Vector2(1.0f / 3.0f,	1.0f / 4.0f),	Vector2(2.0f / 3.0f,1.0f / 4.0f),	Vector2(1.0f / 3.0f,	2.0f / 4.0f),	Vector2(2.0f / 3.0f,2.0f / 4.0f) },
	{	Vector2(2.0f / 3.0f,	1.0f / 4.0f),	Vector2(3.0f / 3.0f,1.0f / 4.0f),	Vector2(2.0f / 3.0f,	2.0f / 4.0f),	Vector2(3.0f / 3.0f,2.0f / 4.0f) },

	// �E����
	{	Vector2(0.0f,			2.0f / 4.0f),	Vector2(1.0f / 3.0f,2.0f / 4.0f),	Vector2(0.0f,		3.0f / 4.0f),	Vector2(1.0f / 3.0f,3.0f / 4.0f) },
	{	Vector2(1.0f / 3.0f,	2.0f / 4.0f),	Vector2(2.0f / 3.0f,2.0f / 4.0f),	Vector2(1.0f / 3.0f,	3.0f / 4.0f),	Vector2(2.0f / 3.0f,3.0f / 4.0f) },
	{	Vector2(2.0f / 3.0f,	2.0f / 4.0f),	Vector2(3.0f / 3.0f,2.0f / 4.0f),	Vector2(2.0f / 3.0f,	3.0f / 4.0f),	Vector2(3.0f / 3.0f,3.0f / 4.0f) },

	// ������
	{	Vector2(0.0f,			3.0f / 4.0f),	Vector2(1.0f / 3.0f,3.0f / 4.0f),	Vector2(0.0f,		4.0f / 4.0f),	Vector2(1.0f / 3.0f,4.0f / 4.0f) },
	{	Vector2(1.0f / 3.0f,	3.0f / 4.0f),	Vector2(2.0f / 3.0f,3.0f / 4.0f),	Vector2(1.0f / 3.0f,	4.0f / 4.0f),	Vector2(2.0f / 3.0f,4.0f / 4.0f) },
	{	Vector2(2.0f / 3.0f,	3.0f / 4.0f),	Vector2(3.0f / 3.0f,3.0f / 4.0f),	Vector2(2.0f / 3.0f,	4.0f / 4.0f),	Vector2(3.0f / 3.0f,4.0f / 4.0f) }
};

// �h���S���A�j���[�V�����e�[�u��(������)
static AnimationTbl	g_DragonAnimDOWN[]
{
	{0,10},
	{1,10},
	{2,10},
	{-1,-1}
};

// �h���S���A�j���[�V�����e�[�u��(������)
static AnimationTbl	g_DragonAnimLEFT[]
{
	{3,10},
	{4,10},
	{5,10},
	{-1,-1}
};

// �h���S���A�j���[�V�����e�[�u��(�E����)
static AnimationTbl	g_DragonAnimRIGHT[]
{
	{6,10},
	{7,10},
	{8,10},
	{-1,-1}
};

// �h���S���A�j���[�V�����e�[�u��(�����)
static AnimationTbl	g_DragonAnimUP[]
{
	{9,10},
	{10,10},
	{11,10},
	{-1,-1}
};

// �A�j���[�V�����e�[�u���S��
static AnimationTbl* g_AnimTableAll[4]
{
	g_DragonAnimDOWN,
	g_DragonAnimLEFT,
	g_DragonAnimRIGHT,
	g_DragonAnimUP
};

// �v���C����������
static void initenemy()
{
	// �h���S��UV���W
	std::array<Vector2,4> uvs = {
		Vector2(0,				0),
		Vector2(1.0f / 3.0f,	0),
		Vector2(0,				1.0f / 4.0f),
		Vector2(1.0f / 3.0f,	1.0f / 4.0f)
	};

	// �}�e���A��
	MATERIAL mtrl{};
	mtrl.Ambient = { 0.0f,0.0f,0.0f,0.0f };
	mtrl.Diffuse = { 0.0f,1.0f,0.0f,1.0f };
	mtrl.Emission = { 0.0f,0.0f,0.0f,0.0f };
	mtrl.Shiness = 0.0f;
	mtrl.Specular = { 0.0f,0.0f,0.0f,0.0f };
	mtrl.TextureEnable = TRUE;

	// �h���S��
	g_spr = std::make_unique<CSprite>(ENEMYWIDTH, ENEMYHEIGHT, "assets/texture/dora01.png", uvs);

	g_spr->ModifyMtrl(mtrl);	// �}�e���A�����Z�b�g
}

static void disposeenemy(int idx)
{

}

static void updateenemy(int idx) 
{
	if (g_enemies[idx].sts < 0) return;	// �X�e�[�^�X�����g�p�Ȃ牽�����Ȃ�)

	static std::random_device rd;				// ����������
	static std::mt19937 mt(rd());				// �����Z���k�E�c�C�X�^�@

	static std::uniform_real_distribution<float> speed(1, 4);
	static std::uniform_int_distribution<int> dir(0, 3);
	static std::uniform_int_distribution<int> counter(10, 20);

	bool keyinputflag = false;
	Vector3	velocity = { 0.0f,0.0f,0.0f };		// �ړ����x

	Enemy& enemy = g_enemies[idx];

	enemy.srt.pos += (enemy.velocity * enemy.speed);	// �X�v���C�g�̈ʒu���X�V����

	enemy.currentAnimTbl = g_AnimTableAll[enemy.direction];

	std::bernoulli_distribution b(0.08f);				// 8%�̊m���Œe����

	if (b(mt))
	{
		FireEnemyBullet(enemy.direction, enemy.srt);
	}
}

static void drawenemy(int idx)
{
	Enemy& enemy = g_enemies[idx];

	if (enemy.sts < 0) return;	// �X�e�[�^�X�����g�p�Ȃ牽�����Ȃ�

	// �w�肵���s�N�`���[�e�[�u���ƃA�j���[�V�����e�[�u���̃C���f�b�N�X�l���g�p���ĉ摜��\������
	UVQuad nowuv;
	nowuv = GetPictureUV(
		enemy.currentAnimIdx,		// ���ݎg�p���Ă���A�j���[�V�����e�[�u���̃C���f�b�N�X�l
		enemy.AnimCounter,			// ���݂̕\���R�}��
		enemy.currentAnimTbl,		// �g�p���Ă���A�j���[�V�����e�[�u��
		DragonPicture);

	g_spr->ModifyUV(nowuv.tex);
	g_spr->Draw(enemy.srt.scale, enemy.srt.rot, enemy.srt.pos);
}


void InitEnemies()
{

	std::random_device rd;										// ����������
	std::mt19937 mt(rd());										// �����Z���k�E�c�C�X�^�@
	std::uniform_real_distribution<float> randX(0, SCREEN_WIDTH);
	std::uniform_real_distribution<float> randY(0, SCREEN_HEIGHT);
	std::uniform_int_distribution<int> randanimcounter(0, 2);

	std::normal_distribution<float> d(	1.0f,					// ����1.0
										0.3f);					// �W���΍�0.3	

	initenemy();

	// �G���̏�����
	resetenemies();

}

void UpdateEnemies()
{
	// �G���̏�����
	for (int i = 0; i < ENEMYMAX; i++)
	{
		updateenemy(i);
	}
}

void DrawEnemies()
{
	// �G���̏�����
	for (int i = 0; i < ENEMYMAX; i++)
	{
		drawenemy(i);
	}
}

void DisposeEnemies()
{
	// �G���̏�����
	for (int i = 0; i < ENEMYMAX; i++)
	{
		disposeenemy(i);
	}
}

std::array<Enemy, ENEMYMAX>& GetEnemies() {
	return g_enemies;
}

void changeenemystatus(int idx, int sts)
{
	g_enemies[idx].sts = sts;		// �X�e�[�^�X�ύX
}

// ���ׂēG�����ɂ܂������H
bool isallDead() {

	bool sts = true;

	for (auto& e : g_enemies) 
	{
		if (e.sts != -1) {
			sts = false;
			break;
		}
	}

	return sts;
}

// ���ׂēG�����ɂ܂������H
void resetenemies() 
{

	std::random_device rd;										// ����������
	std::mt19937 mt(rd());										// �����Z���k�E�c�C�X�^�@
	std::uniform_real_distribution<float> randX(0, SCREEN_WIDTH);
	std::uniform_real_distribution<float> randY(0, SCREEN_HEIGHT);
	std::uniform_int_distribution<int> randanimcounter(0, 2);

	std::normal_distribution<float> d(1.0f,					// ����1.0
		0.3f);					// �W���΍�0.3	

	// �G���̏�����
	for (auto& e : g_enemies)
	{
		e.sts = 1;						// �X�e�[�^�X�i�[�P�F���g�p�@�P�F�g�p���@�j	
		e.direction = 1;				// ����
		e.speed = 0;					// �ړ��X�s�[�h
		e.velocity = Vector3(0, 0, 0);	// �ړ����x

		e.srt.pos.x = randX(mt);		// �X�v���C�g�̈ʒu
		e.srt.pos.y = randY(mt);		// �X�v���C�g�̈ʒu
		e.srt.pos.z = 0.0f;

		e.srt.rot = Vector3(0, 0, 0);
		e.srt.scale.x = e.srt.scale.y = e.srt.scale.z = d(mt);		// �X�v���C�g�̊g�嗦	

		// �A�j���[�V�������
		e.currentAnimIdx = 0;					// �A�j���[�V�����e�[�u���̃C���f�b�N�X
		e.currentAnimTbl = g_AnimTableAll[0];	// ���ݎg�p���̃A�j���[�V�����e�[�u��
		e.AnimCounter = randanimcounter(mt);	// �\���R�}��

		e.movecount = 0;						// �ړ��J�E���g
	}
}