#pragma once

#include <string>
#include <wrl/client.h>
#include <vector>

#pragma warning(push)
#pragma warning(disable:4005)

#include <d2d1.h>						// Direct2D
#include <dwrite_3.h>					// DirectWrite(�O���t�H���g�g�p�̂���
#include "CommonTypes.h"			// ���w�n���C�u����

#pragma warning(pop)

#pragma comment(lib,"d2d1.lib")			// Direct2D�p
#pragma comment(lib,"Dwrite.lib")		// DirectWrite�p

using Microsoft::WRL::ComPtr;

class CustomFontCollectionLoader;

//�g�p����t�H���g���������ɒǉ�����
namespace FontList
{
	enum class FontName
	{
		MelodyLine,
		Google,
	};
}

//��������������������������
//		�t�H���g�f�[�^
//��������������������������
struct FontData
{
	FontList::FontName font;							// �t�H���g��
	DWRITE_FONT_WEIGHT fontWeight;						// �t�H���g�̑���
	DWRITE_FONT_STYLE fontStyle;						// �t�H���g�X�^�C��
	DWRITE_FONT_STRETCH fontStretch;					// �t�H���g�̕�
	FLOAT fontSize;										// �t�H���g�T�C�Y
	std::wstring localeName;							// ���P�[����
	DWRITE_TEXT_ALIGNMENT textAlignment;				// �e�L�X�g�̔z�u
	D2D1_COLOR_F Color;									// �t�H���g�̐F

	D2D1_COLOR_F shadowColor;							// �e�̐F
	D2D1_POINT_2F shadowOffset;							// �e�̃I�t�Z�b�g

	// �f�t�H���g�ݒ�i�R���X�g���N�^�j
	FontData()
	{
		font = FontList::FontName::Google;
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;
		fontStyle = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
		fontStretch = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL;
//		fontSize = 40;
		fontSize = 10;
		localeName = L"ja-jp";
		textAlignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
		Color = D2D1::ColorF(D2D1::ColorF::White);

		shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
		shadowOffset = D2D1::Point2F(2.0f, -2.0f);
	}
};

//������������������������������
//		DirectWrite�N���X
//������������������������������
class DirectWrite
{
private:
	ComPtr <ID2D1Factory>			m_cpD2DFactory = nullptr;		// Direct2D���\�[�X
	ComPtr <ID2D1RenderTarget>		m_cpRenderTarget = nullptr;		// Direct2D�����_�[�^�[�Q�b�g
	ComPtr <ID2D1SolidColorBrush>	m_cpBrush = nullptr;			// Direct2D�u���V�ݒ�
	ComPtr <ID2D1SolidColorBrush>	m_cpShadowBrush = nullptr;		// Direct2D�u���V�ݒ�i�e�j
	ComPtr <IDWriteFactory5>		m_cpDWriteFactory;				// DirectWrite���\�[�X
	ComPtr <IDWriteTextFormat>		m_cpTextFormat = nullptr;		// DirectWrite�e�L�X�g�`��
	ComPtr <IDWriteTextLayout>		m_cpTextLayout = nullptr;		// DirectWrite�e�L�X�g����
	ComPtr <IDXGISurface>			m_cpBackBuffer = nullptr;		// �T�[�t�F�X���

	static ComPtr <CustomFontCollectionLoader> pFontCollectionLoader;

	std::wstring m_CurrentText;										//���ݕ`�悵�Ă��镶����ۑ�����ϐ�

	// �t�H���g�t�@�C�����X�g
	std::vector<ComPtr<IDWriteFontFile>> m_cpFontFileList;

	// �t�H���g�f�[�^
	FontData  m_Setting;

	// �t�H���g�����X�g
	std::vector<std::wstring> m_FontNamesList;

	// �t�H���g�̃t�@�C�������擾����
	std::wstring GetFontFileNameWithoutExtension(const std::wstring& _filePath);

	// string��wstring�֕ϊ�����
	std::wstring StringToWString(std::string _oString);

	//�����\�����鎞�Ԃ̕ێ�����ϐ�
	float m_TotalTime = 0.0f;

public:
	// �f�t�H���g�R���X�g���N�^�𐧌�
	DirectWrite()
	{

	}

	// �R���X�g���N�^
	// ��1�����F�t�H���g�ݒ�
	DirectWrite(FontData* _set) :m_Setting(*_set) {};

	// �������֐�
	HRESULT Init(IDXGISwapChain* _swapChain);

	/// <summary>
	/// �t�H���g�f�[�^�\����
	/// </summary>
	void SetFont(FontData _set);

	/// <summary>
	/// // �t�H���g�ݒ�
	// ��1�����F�t�H���g���iL"���C���I", L"Arial", L"Meiryo UI"���j
	// ��2�����F�t�H���g�̑����iDWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT_BOLD���j
	// ��3�����F�t�H���g�X�^�C���iDWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STYLE_ITALIC�j
	// ��4�����F�t�H���g�̕��iDWRITE_FONT_STRETCH_NORMAL,DWRITE_FONT_STRETCH_EXTRA_EXPANDED���j
	// ��5�����F�t�H���g�T�C�Y�i20, 30���j
	// ��6�����F���P�[�����iL"ja-jp"���j
	// ��7�����F�e�L�X�g�̔z�u�iDWRITE_TEXT_ALIGNMENT_LEADING�F�O, ���j
	// ��8�����F�t�H���g�̐F�iD2D1::ColorF(D2D1::ColorF::Black)�F��, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))�FRGBA�w�蓙�j
	// ��9�����F�e�̐F�iD2D1::ColorF(D2D1::ColorF::Black)�F��, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))�FRGBA�w�蓙�j
	// ��10�����F�e�̃I�t�Z�b�g�iD2D1::Point2F(2.0f, 2.0f)�F�E���Ƀ|�C���g���炷�j
	/// </summary>
	void SetFonts
	(
		FontList::FontName		_fontname,											// �t�H���g��
		DWRITE_FONT_WEIGHT		_fontWeight = DWRITE_FONT_WEIGHT_NORMAL,			// �t�H���g�̑���
		DWRITE_FONT_STYLE		_fontStyle = DWRITE_FONT_STYLE_NORMAL,				// �t�H���g�X�^�C��
		DWRITE_FONT_STRETCH		_fontStretch = DWRITE_FONT_STRETCH_NORMAL,			// �t�H���g�̕�
		FLOAT					_fontSize = 20,										// �t�H���g�T�C�Y
		std::wstring _localeName = L"ja-jp",										// ���P�[����
		DWRITE_TEXT_ALIGNMENT	_textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING,		// �e�L�X�g�̔z�u
		D2D1_COLOR_F			_Color = D2D1::ColorF(D2D1::ColorF::White),			// �t�H���g�̐F
		D2D1_COLOR_F			_shadowColor = D2D1::ColorF(D2D1::ColorF::Black),	// �e�̐F
		D2D1_POINT_2F			_shadowOffset = D2D1::Point2F(2.0f, -2.0f)			// �e�̃I�t�Z�b�g
	);

	/// <summary>
	/// �����`��
	// string�F������
	// pos�F�`��|�W�V����
	// options�F�e�L�X�g�̐��`
	/// </summary>
	HRESULT DrawString(std::wstring _str, DirectX::SimpleMath::Vector2 _pos,
		D2D1_DRAW_TEXT_OPTIONS _options = D2D1_DRAW_TEXT_OPTIONS_NONE, bool _shadow = false);


	/// <summary>
	/// �������ꕶ�����`��
	/// </summary>
	/// <param name="_num"></param>
	/// <returns></returns>
	HRESULT DrawStringPartial(std::wstring _str, DirectX::SimpleMath::Vector2 _pos,
		bool _shadow = false,bool _changeText = true ,float _delaytime = 0.2f,
		float _elapsedTime = 0.1f,
		D2D1_DRAW_TEXT_OPTIONS _options = D2D1_DRAW_TEXT_OPTIONS_NONE
		);

	// �����`��
	// string�F������
	// rect�F�̈�w��
	// options�F�e�L�X�g�̐��`
	HRESULT DrawString(std::string _str, D2D1_RECT_F _rect,
		D2D1_DRAW_TEXT_OPTIONS _options, bool _shadow = false);

	// �w�肳�ꂽ�p�X�̃t�H���g��ǂݍ���
	HRESULT FontLoader();

	// �t�H���g�����擾����
	std::wstring GetFontName(int _num);

	// �ǂݍ��񂾃t�H���g���̐����擾����
	int GetFontNameNum();

	// �t�H���g�����擾������
	HRESULT GetFontFamilyName(IDWriteFontCollection* _customFontCollection,
		std::wstring _locale = L"ja-jp");

	// �S�Ẵt�H���g�����擾������
	HRESULT GetAllFontFamilyName(IDWriteFontCollection* _customFontCollection);

	/// <summary>
	/// �t�H���g�t�@�C�����X�g�ɊO���t�H���g��ǉ�����
	/// </summary>
	HRESULT LoadFontFiles(const std::vector<std::wstring>& fontPaths);

	// �J�X�^���t�H���g�R���N�V����
	ComPtr <IDWriteFontCollection> fontCollection = nullptr;

	/// <summary>
	/// �V�[���`�F���W����Ƃ��ɌĂяo����
	/// ������x�ꕶ�����\������
	/// </summary>
	void ResetText();

	//�ꕔ�̕����̃t�H���g��ύX
	HRESULT ChangeTextFont(IDWriteTextLayout* _textLayout,
		FontList::FontName _fontName,  UINT32 _startChar, UINT32 _endChar)
	{
		return _textLayout->SetFontFamilyName(GetFontName((int)_fontName).c_str(),
			{_startChar,_endChar});
	}

	//static void ReleaseFontCollectionLoader();

	~DirectWrite();
};

const std::wstring FontPath[] =
{
	L"Assets\\font\\MelodyLine-free.otf",			//�����f�B�[���C��
	L"Assets\\font\\NotoSansJP-Black.otf",			//Google���{�ꕶ��
};

//=============================================================================
//		�J�X�^���t�@�C�����[�_�[
//=============================================================================
class CustomFontFileEnumerator : public IDWriteFontFileEnumerator
{
public:
	CustomFontFileEnumerator(ComPtr<IDWriteFactory> factory, const std::vector<std::wstring>& fontFilePaths)
		: factory_(factory), fontFilePaths_(fontFilePaths), currentFileIndex_(-1)
	{
	}

	~CustomFontFileEnumerator() = default;

	IFACEMETHODIMP QueryInterface(REFIID iid, void** ppvObject) override
	{
		if (iid == __uuidof(IUnknown) || iid == __uuidof(IDWriteFontFileEnumerator))
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}

	IFACEMETHODIMP_(ULONG) AddRef() override
	{
		return InterlockedIncrement(&refCount_);
	}

	IFACEMETHODIMP_(ULONG) Release() override
	{
		ULONG newCount = InterlockedDecrement(&refCount_);
		if (newCount == 0)
		{
			delete this;
		}
		else if (newCount > static_cast<ULONG>(-1)) // �I�[�o�[�t���[�΍�
		{
			__debugbreak(); // �f�o�b�O�p�ɒ�~
		}

		return newCount;
	}

	IFACEMETHODIMP MoveNext(OUT BOOL* hasCurrentFile) noexcept override
	{
		*hasCurrentFile = (++currentFileIndex_ < static_cast<int>(fontFilePaths_.size())) ? TRUE : FALSE;
		return S_OK;
	}

	IFACEMETHODIMP GetCurrentFontFile(OUT IDWriteFontFile** fontFile) noexcept override
	{
		if (currentFileIndex_ < 0 || currentFileIndex_ >= static_cast<int>(fontFilePaths_.size()))
		{
			*fontFile = nullptr;
			return E_FAIL;
		}

		try
		{
			return factory_->CreateFontFileReference(fontFilePaths_[currentFileIndex_].c_str(), nullptr, fontFile);
		}
		catch (const std::exception&)
		{
			*fontFile = nullptr;
			return E_FAIL;
		}
	}

private:
	ULONG refCount_ = 0;
	ComPtr<IDWriteFactory> factory_;   // ComPtr ���g�p���Ď����I�ɊǗ�
	std::vector<std::wstring> fontFilePaths_;
	int currentFileIndex_ = -1;
};


//=============================================================================
//		�J�X�^���t�H���g�R���N�V�������[�_�[
//=============================================================================
class CustomFontCollectionLoader : public IDWriteFontCollectionLoader
{
public:
	// �R���X�g���N�^
	CustomFontCollectionLoader() : refCount_(0) {}

	// IUnknown ���\�b�h
	IFACEMETHODIMP QueryInterface(REFIID iid, void** ppvObject) override
	{
		if (ppvObject == nullptr)
			return E_POINTER;

		if (iid == __uuidof(IUnknown) || iid == __uuidof(IDWriteFontCollectionLoader))
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
		{
			*ppvObject = nullptr;
			return E_NOINTERFACE;
		}
	}

	IFACEMETHODIMP_(ULONG) AddRef() override
	{
		return InterlockedIncrement(&refCount_);
	}

	IFACEMETHODIMP_(ULONG) Release() override
	{
		ULONG newCount = InterlockedDecrement(&refCount_);
		if (newCount == 0)
		{
			delete this;
		}
		else if (newCount > static_cast<ULONG>(-1)) // �I�[�o�[�t���[�΍�
		{
			__debugbreak(); // �f�o�b�O�p�ɒ�~
		}

		return newCount;
	}

	// IDWriteFontCollectionLoader ���\�b�h
	IFACEMETHODIMP CreateEnumeratorFromKey
	(
		IDWriteFactory* factory,
		void const* collectionKey,
		UINT32 collectionKeySize,
		OUT IDWriteFontFileEnumerator** fontFileEnumerator
	) noexcept override
	{
		if (!factory || !fontFileEnumerator || !collectionKey || collectionKeySize == 0)
		{
			return E_POINTER;
		}

		std::vector<std::wstring> fontFilePaths(std::begin(FontPath), std::end(FontPath));

		// new �ł͂Ȃ��Q�ƃJ�E���g�Ǘ�
		ComPtr<CustomFontFileEnumerator> enumerator =
			new (std::nothrow) CustomFontFileEnumerator(factory, fontFilePaths);

		if (!enumerator)
		{
			return E_OUTOFMEMORY;
		}

		*fontFileEnumerator = enumerator.Detach(); // ComPtr ���珊�L�����ړ�
		return S_OK;
	}


private:
	ULONG refCount_; // �Q�ƃJ�E���g
};