#pragma once

/**
 * @file Renderer.h
 * @brief DirectX�����_�����O�Ŏg�p����e��\���̂�Renderer�N���X�̐錾
 */

#include "CommonTypes.h"
#include "Transform.h"
#include "NonCopyable.h"
#include <d3d11.h>
#include <io.h>
#include <string>
#include <vector>
#include <wrl/client.h>

 // �����N���ׂ��O�����C�u����
//#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")

/**
 * @struct WEIGHT
 * @brief �{�[���̉e������ێ�����\����
 * @date 20231225 �ǉ�
 */
struct WEIGHT {
    std::string bonename;   ///< �{�[����
    std::string meshname;   ///< ���b�V����
    float weight;           ///< �E�F�C�g�l
    int vertexindex;        ///< ���_�C���f�b�N�X
};

/**
 * @struct BONE
 * @brief �{�[���\���́iDX�Ή��Łj
 * @date 20231231 �C���iDX���j
 */
struct BONE
{
    std::string bonename;          ///< �{�[����
    std::string meshname;          ///< ���b�V����
    std::string armaturename;      ///< �A�[�}�`���A��
    Matrix4x4 Matrix{};            ///< �e�q�֌W���l�������s��
    Matrix4x4 AnimationMatrix{};   ///< �����̕ό`�݂̂��l�������s��
    Matrix4x4 OffsetMatrix{};      ///< �{�[���I�t�Z�b�g�s��
    int idx;                       ///< �z�񒆂̃C���f�b�N�X
    std::vector<WEIGHT> weights;   ///< ���̃{�[�����e����^���钸�_�ƃE�F�C�g�l�̃��X�g
};

/**
 * @struct VERTEX_3D
 * @brief �R�������_�f�[�^���i�[����\����
 */
struct VERTEX_3D
{
    Vector3 Position;            ///< ���_�̍��W
    Vector3 Normal;              ///< �@���x�N�g��
    Color Diffuse;               ///< �g�U���ːF
    Vector2 TexCoord;            ///< �e�N�X�`�����W
    int BoneIndex[4];            ///< �{�[���C���f�b�N�X�i�ő�4�j 20231225
    float BoneWeight[4];         ///< �e�{�[���̃E�F�C�g�l 20231225
    std::string BoneName[4];     ///< �e�{�[���̖��O 20231226
    int bonecnt = 0;             ///< �e����^����{�[���� 20231226
};

/**
 * @struct MATERIAL
 * @brief �}�e���A������ێ�����\����
 */
struct MATERIAL
{
    Color Ambient;         ///< �A���r�G���g�F
    Color Diffuse;         ///< �g�U�F
    Color Specular;        ///< ���ʔ��ːF
    Color Emission;        ///< ���Ȕ����F
    float Shiness;         ///< ����x
    BOOL TextureEnable;    ///< �e�N�X�`���g�p�t���O
    float Dummy[2]{};      ///< �\���̈�
};

/**
 * @struct LIGHT
 * @brief ���s�����̏���ێ�����\����
 */
struct LIGHT
{
    BOOL Enable;           ///< ���C�g�̗L��/�����t���O
    BOOL Dummy[3];         ///< �p�f�B���O�p�i�_�~�[�j
    Vector4 Direction;     ///< ���̕���
    Color Diffuse;         ///< �g�U���̐F
    Color Ambient;         ///< �����̐F
};

/**
 * @struct SUBSET
 * @brief ���b�V���̃T�u�Z�b�g�i�}�e���A�����j����ێ�����\����
 */
struct SUBSET {
    std::string MtrlName;      ///< �}�e���A����
    unsigned int IndexNum = 0; ///< �C���f�b�N�X��
    unsigned int VertexNum = 0;///< ���_��
    unsigned int IndexBase = 0;///< �J�n�C���f�b�N�X
    unsigned int VertexBase = 0;///< ���_�x�[�X
    unsigned int MaterialIdx = 0;///< �}�e���A���C���f�b�N�X
};

/**
 * @enum EBlendState
 * @brief �u�����h�X�e�[�g�̎��
 */
enum EBlendState {
    BS_NONE = 0,      ///< ��������������
    BS_ALPHABLEND,    ///< ����������
    BS_ADDITIVE,      ///< ���Z����
    BS_SUBTRACTION,   ///< ���Z����
    MAX_BLENDSTATE    ///< �u�����h�X�e�[�g�̍ő�l
};

/**
 * @struct CBBoneCombMatrix
 * @brief �{�[���R���r�l�[�V�����s���ێ�����\����
 * @date 20240713
 */
constexpr int MAX_BONE = 400;
struct CBBoneCombMatrix {
    DirectX::XMFLOAT4X4 BoneCombMtx[MAX_BONE];  ///< �{�[���R���r�l�[�V�����s��̔z��
};

/**
 * @class Renderer
 * @brief DirectX�����_�����O�������Ǘ����郌���_���N���X
 *
 * ���̃N���X�́ADirect3D�f�o�C�X�A�R���e�L�X�g�A�X���b�v�`�F�[���Ȃǂ̊Ǘ��ƁA
 * �����_�����O�����̏������A�J�n�A�I���Ȃǂ̋@�\��񋟂��܂��B
 */
class Renderer : NonCopyable
{
private:
    static D3D_FEATURE_LEVEL m_FeatureLevel;

    static ComPtr<ID3D11Device> m_Device;
    static ComPtr<ID3D11DeviceContext> m_DeviceContext;
    static ComPtr<IDXGISwapChain> m_SwapChain;
    static ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
    static ComPtr<ID3D11DepthStencilView> m_DepthStencilView;

    static ComPtr<ID3D11Buffer> m_WorldBuffer;
    static ComPtr<ID3D11Buffer> m_ViewBuffer;
    static ComPtr<ID3D11Buffer> m_ProjectionBuffer;
    static ComPtr<ID3D11Buffer> m_MaterialBuffer;
    static ComPtr<ID3D11Buffer> m_LightBuffer;

    static ComPtr<ID3D11DepthStencilState> m_DepthStateEnable;
    static ComPtr<ID3D11DepthStencilState> m_DepthStateDisable;

    static ComPtr<ID3D11BlendState> m_BlendState[MAX_BLENDSTATE];
    static ComPtr<ID3D11BlendState> m_BlendStateATC;

public:
    static void Init();
    static void Uninit();
    static void Begin();
    static void End();
    static void SetDepthEnable(bool Enable);
    static void SetDepthAllwaysWrite();
    static void SetATCEnable(bool Enable);
    static void SetWorldViewProjection2D();
    static void SetWorldMatrix(Matrix4x4* WorldMatrix);
    static void SetViewMatrix(Matrix4x4* ViewMatrix);
    static void SetProjectionMatrix(Matrix4x4* ProjectionMatrix);
    static void SetMaterial(MATERIAL Material);
    static void SetLight(LIGHT Light);
    static ID3D11Device* GetDevice(void) { return m_Device.Get(); }
    static ID3D11DeviceContext* GetDeviceContext(void) { return m_DeviceContext.Get(); }
    static void SetBlendState(int nBlendState);
    static IDXGISwapChain* GetSwapChain() { return m_SwapChain.Get(); }
    static void ClearDepthBuffer() {
        m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    }
    static void DisableCulling(bool cullflag = false);
    static void SetFillMode(D3D11_FILL_MODE FillMode);
};