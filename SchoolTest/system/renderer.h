#pragma once

/**
 * @file Renderer.h
 * @brief DirectX�����_�����O�Ŏg�p����e��\���̂�Renderer�N���X�̐錾
 */

#include "CommonTypes.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <io.h>
#include <string>
#include <vector>
#include <wrl/client.h>
#include "NonCopyable.h"
#include "Transform.h"


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
    BOOL  TextureEnable;    ///< �e�N�X�`���g�p�t���O
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
    static D3D_FEATURE_LEVEL       m_FeatureLevel;         ///< Direct3D�̃t�B�[�`���[���x��

    static ID3D11Device* m_Device;               ///< Direct3D�f�o�C�X
    static ID3D11DeviceContext* m_DeviceContext;        ///< Direct3D�f�o�C�X�R���e�L�X�g
    static IDXGISwapChain* m_SwapChain;            ///< �X���b�v�`�F�[��
    static ID3D11RenderTargetView* m_RenderTargetView;     ///< �����_�[�^�[�Q�b�g�r���[
    static ID3D11DepthStencilView* m_DepthStencilView;     ///< �[�x/�X�e���V���r���[

    static ID3D11Buffer* m_WorldBuffer;          ///< ���[���h�s��p�萔�o�b�t�@
    static ID3D11Buffer* m_ViewBuffer;           ///< �r���[�s��p�萔�o�b�t�@
    static ID3D11Buffer* m_ProjectionBuffer;     ///< �ˉe�s��p�萔�o�b�t�@
    static ID3D11Buffer* m_MaterialBuffer;       ///< �}�e���A�����p�萔�o�b�t�@
    static ID3D11Buffer* m_LightBuffer;          ///< ���C�g���p�萔�o�b�t�@

    static ID3D11DepthStencilState* m_DepthStateEnable;    ///< �[�x�e�X�g�L�����̐[�x�X�e���V�����
    static ID3D11DepthStencilState* m_DepthStateDisable;   ///< �[�x�e�X�g�������̐[�x�X�e���V�����

    static ID3D11BlendState* m_BlendState[MAX_BLENDSTATE]; ///< �u�����h�X�e�[�g�z��
    static ID3D11BlendState* m_BlendStateATC;        ///< ATC�p�u�����h�X�e�[�g

public:
    /**
     * @brief �����_���[�̏������������s���܂��B
     */
    static void Init();

    /**
     * @brief �����_���[�̏I���������s���A�m�ۂ������\�[�X��������܂��B
     */
    static void Uninit();

    /**
     * @brief �`�揈���̊J�n��錾���܂��B
     */
    static void Begin();

    /**
     * @brief �`�揈���̏I����錾���܂��B
     */
    static void End();

    /**
     * @brief �[�x�e�X�g�̗L��/������ݒ肵�܂��B
     * @param Enable true�ŗL���Afalse�Ŗ���
     */
    static void SetDepthEnable(bool Enable);

    /**
     * @brief �[�x�e�X�g����Ƀp�X������ݒ�ɕύX���܂��B
     *
     * �[�x�e�X�g��L���ɂ��A��r�֐���ALWAYS�ɁA�������݃}�X�N��S�r�b�g�ɐݒ肵�܂��B
     */
    static void SetDepthAllwaysWrite() {
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
        ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

        // �[�x�e�X�g��L���ɂ��A��ɕ`�悷��悤�ɐݒ�
        depthStencilDesc.DepthEnable = TRUE;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

        // �X�e���V���e�X�g�𖳌���
        depthStencilDesc.StencilEnable = FALSE;

        ComPtr<ID3D11DepthStencilState> pDepthStencilState;
        m_Device->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);
        m_DeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 0);
    }

    /**
     * @brief ATC�iAlpha-to-Coverage�j�̗L��/������ݒ肵�܂��B
     * @param Enable true�ŗL���Afalse�Ŗ���
     */
    static void SetATCEnable(bool Enable);

    /**
     * @brief 2D�`��p�̃��[���h�E�r���[�E�ˉe�s���ݒ肵�܂��B
     */
    static void SetWorldViewProjection2D();

    /**
     * @brief ���[���h�s���ݒ肵�܂��B
     * @param WorldMatrix ���[���h�s��ւ̃|�C���^
     */
    static void SetWorldMatrix(Matrix4x4* WorldMatrix);

    /**
     * @brief �r���[�s���ݒ肵�܂��B
     * @param ViewMatrix �r���[�s��ւ̃|�C���^
     */
    static void SetViewMatrix(Matrix4x4* ViewMatrix);

    /**
     * @brief �ˉe�s���ݒ肵�܂��B
     * @param ProjectionMatrix �ˉe�s��ւ̃|�C���^
     */
    static void SetProjectionMatrix(Matrix4x4* ProjectionMatrix);

    /**
     * @brief �}�e���A������ݒ肵�܂��B
     * @param Material �}�e���A�����
     */
    static void SetMaterial(MATERIAL Material);

    /**
     * @brief ���C�g����ݒ肵�܂��B
     * @param Light ���C�g���
     */
    static void SetLight(LIGHT Light);

    /**
     * @brief Direct3D�f�o�C�X���擾���܂��B
     * @return Direct3D�f�o�C�X�ւ̃|�C���^
     */
    static ID3D11Device* GetDevice(void) { return m_Device; }

    /**
     * @brief Direct3D�f�o�C�X�R���e�L�X�g���擾���܂��B
     * @return Direct3D�f�o�C�X�R���e�L�X�g�ւ̃|�C���^
     */
    static ID3D11DeviceContext* GetDeviceContext(void) { return m_DeviceContext; }

    /**
     * @brief �w�肵���u�����h�X�e�[�g��ݒ肵�܂��B
     * @param nBlendState �u�����h�X�e�[�g�̃C���f�b�N�X
     */
    static void SetBlendState(int nBlendState)
    {
        if (nBlendState >= 0 && nBlendState < MAX_BLENDSTATE) {
            float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            m_DeviceContext->OMSetBlendState(m_BlendState[nBlendState], blendFactor, 0xffffffff);
        }
    }

    /**
     * @brief �X���b�v�`�F�[�����擾���܂��B
     * @return IDXGISwapChain�ւ̃|�C���^
     */
    static IDXGISwapChain* GetSwapChain() {
        return m_SwapChain;
    }

    /**
     * @brief �[�x�o�b�t�@���N���A���܂��B
     */
    static void ClearDepthBuffer() {
        m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    /**
     * @brief �J�����O�i�ʂ̏��O�j�𖳌��܂��͗L���ɐݒ肵�܂��B
     * @param cullflag true�ŃJ�����O�L���Afalse�ŃJ�����O�����i�f�t�H���g��false�j
     */
    static void DisableCulling(bool cullflag = false)
    {
        // ���X�^���C�U�̐ݒ���쐬
        D3D11_RASTERIZER_DESC rasterizerDesc;
        ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
        rasterizerDesc.FillMode = D3D11_FILL_SOLID;  // �\���b�h�h��Ԃ�

        // cullflag�ɉ������J�����O���[�h�̐ݒ�
        rasterizerDesc.CullMode = cullflag ? D3D11_CULL_BACK : D3D11_CULL_NONE;

        rasterizerDesc.FrontCounterClockwise = FALSE; // �������̒��_�������v���
        rasterizerDesc.DepthBias = 0;
        rasterizerDesc.SlopeScaledDepthBias = 0.0f;
        rasterizerDesc.DepthClipEnable = TRUE;  // �[�x�N���b�s���O�L��
        rasterizerDesc.ScissorEnable = FALSE;   // �V�U�[����
        rasterizerDesc.MultisampleEnable = FALSE; // �}���`�T���v������
        rasterizerDesc.AntialiasedLineEnable = FALSE; // �A���`�G�C���A�X���C������

        // ���X�^���C�U�X�e�[�g�̍쐬
        ID3D11RasterizerState* pRasterizerState = nullptr;
        HRESULT hr = m_Device->CreateRasterizerState(&rasterizerDesc, &pRasterizerState);
        if (FAILED(hr))
        {
            return; // �쐬�Ɏ��s�����ꍇ�͏������I��
        }

        // �����_�[�R���e�L�X�g�փ��X�^���C�U�X�e�[�g��ݒ�
        m_DeviceContext->RSSetState(pRasterizerState);

        // �쐬�����X�e�[�g�̉��
        pRasterizerState->Release();
        return;
    }
};