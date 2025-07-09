#include "DirectWrite.h"
#include "Application.h"
#include <assert.h>

//����������������������������������
//	Windows11�ł�.otf�̕��𐄏�
//	.ttf�Ƃ͑���������
//����������������������������������


//ComPtr <IDWriteFactory5>	DirectWrite::m_cpDWriteFactory;
ComPtr <CustomFontCollectionLoader> DirectWrite::pFontCollectionLoader;

std::wstring DirectWrite::GetFontFileNameWithoutExtension(const std::wstring& _filePath)
{
	// �������猟�����ăt�@�C�����Ɗg���q�̈ʒu���擾
	size_t start = _filePath.find_last_of(L"/\\") + 1;
	size_t end = _filePath.find_last_of(L'.');

	// �t�@�C�������擾���ĕԂ�
	return _filePath.substr(start, end - start);
}

std::wstring DirectWrite::StringToWString(std::string _oString)
{
	// �K�v�ȃo�b�t�@�T�C�Y���v�Z���āA���̂܂�wstring�ɕϊ�
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, _oString.c_str(), -1, nullptr, 0);

	// �o�b�t�@���m�ۂ��A����wstring�ɕϊ�
	std::wstring oRet(iBufferSize - 1, L'\0');
	MultiByteToWideChar(CP_ACP, 0, _oString.c_str(), -1, &oRet[0], iBufferSize);

	// �ϊ����ʂ�Ԃ�
	return oRet;
}

HRESULT DirectWrite::Init(IDXGISwapChain* _swapChain)
{
	// �o�b�N�o�b�t�@�̃��Z�b�g
	m_cpBackBuffer.Reset();

	// �o�b�N�o�b�t�@�̎擾
	HRESULT result = _swapChain->GetBuffer(0, IID_PPV_ARGS(&m_cpBackBuffer));
	assert(SUCCEEDED(result) && "�o�b�N�o�b�t�@�̎擾���s");

	// Direct2D�t�@�N�g�����̏�����
	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_cpD2DFactory.GetAddressOf());
	assert(SUCCEEDED(result) && "Direct2D�t�@�N�g�����̏��������s");

	// �X�N���[���̃T�C�Y���擾
	FLOAT dpiX = (FLOAT)Application::GetWidth();
	FLOAT dpiY = (FLOAT)Application::GetHeight();

	// �����_�[�^�[�Q�b�g�̍쐬
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// �T�[�t�F�X�ɕ`�悷�郌���_�[�^�[�Q�b�g���쐬
	result = m_cpD2DFactory->CreateDxgiSurfaceRenderTarget(m_cpBackBuffer.Get(), &props,
		m_cpRenderTarget.GetAddressOf());
	assert(SUCCEEDED(result) && "�T�[�t�F�X�ɕ`�悷�郌���_�[�^�[�Q�b�g���쐬���s");

	// �A���`�G�C���A�V���O���[�h�̐ݒ�
	m_cpRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

	// IDWriteFactory�̍쐬
	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(m_cpDWriteFactory.GetAddressOf()));
	assert(SUCCEEDED(result) && "IDWriteFactory�̍쐬���s");

	// �J�X�^���t�H���g�R���N�V�������[�_�[�̃C���X�^���X�쐬
	pFontCollectionLoader = ComPtr<CustomFontCollectionLoader>(new (std::nothrow) CustomFontCollectionLoader());
	if (!pFontCollectionLoader) {
		// �C���X�^���X�쐬���s
		return E_FAIL;
	}

	// �J�X�^���t�H���g�R���N�V�������[�_�[��o�^
	result = m_cpDWriteFactory->RegisterFontCollectionLoader(pFontCollectionLoader.Get());
	assert(SUCCEEDED(result) && "�J�X�^���t�H���g�R���N�V�������[�_�[��o�^���s");

	// �t�H���g�t�@�C���̓ǂݍ���
	result = FontLoader();
	assert(SUCCEEDED(result));

	// �t�H���g��ݒ�
	SetFont(m_Setting);

	return result;
}


void DirectWrite::SetFont(FontData _set)
{
	HRESULT result = S_OK;

	// �ݒ���R�s�[
	m_Setting = _set;

	// �O��쐬�����u���V�����
	m_cpBrush.Reset();
	m_cpShadowBrush.Reset();

	// �t�H�[�}�b�g�����
	m_cpTextFormat.Reset();

	// �t�H���g�����擾����
	std::wstring name = L"";
	int num = (int)m_Setting.font;
	ComPtr <IDWriteFontCollection> fc = fontCollection;
	if (num >= (int)m_FontNamesList.size() || m_FontNamesList.empty()) {
		// num��font�̎�ނ��傫�� or �t�H���g���̃��X�g���󂾂�����Ƃ肠�����擪��font���̗p
		if (num >= (int)FontPath->size()) {

			name = m_FontNamesList[0];
		}
		else {
			name = FontPath[num];
			fc = nullptr;
		}
	}
	else
	{
		name = m_FontNamesList[num];
	}


	//�֐�CreateTextFormat()
	//��1�����F�t�H���g���iL"���C���I", L"Arial", L"Meiryo UI"���j
	//��2�����F�t�H���g�R���N�V�����inullptr�j
	//��3�����F�t�H���g�̑����iDWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT_BOLD���j
	//��4�����F�t�H���g�X�^�C���iDWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STYLE_ITALIC�j
	//��5�����F�t�H���g�̕��iDWRITE_FONT_STRETCH_NORMAL,DWRITE_FONT_STRETCH_EXTRA_EXPANDED���j
	//��6�����F�t�H���g�T�C�Y�i20, 30���j
	//��7�����F���P�[�����iL""�j
	//��8�����F�e�L�X�g�t�H�[�}�b�g�i&g_pTextFormat�j
	result = m_cpDWriteFactory->CreateTextFormat(
		name.c_str(),
		fc.Get(),
		m_Setting.fontWeight,
		m_Setting.fontStyle,
		m_Setting.fontStretch,
		m_Setting.fontSize,
		m_Setting.localeName.c_str(),
		m_cpTextFormat.GetAddressOf()
	);
	assert(SUCCEEDED(result) && "�e�L�X�g�t�H�[�}�b�g�쐬���s");

	//�֐�SetTextAlignment()
	//��1�����F�e�L�X�g�̔z�u�iDWRITE_TEXT_ALIGNMENT_LEADING�F�O, DWRITE_TEXT_ALIGNMENT_TRAILING�F��, DWRITE_TEXT_ALIGNMENT_CENTER�F����,
	//                         DWRITE_TEXT_ALIGNMENT_JUSTIFIED�F�s�����ς��j
	result = m_cpTextFormat->SetTextAlignment(m_Setting.textAlignment);
	assert(SUCCEEDED(result) && "�e�L�X�g�̔z�u���s");

	//�֐�CreateSolidColorBrush()
	//��1�����F�t�H���g�F�iD2D1::ColorF(D2D1::ColorF::Black)�F��, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))�FRGBA�w��j
	result = m_cpRenderTarget->CreateSolidColorBrush(m_Setting.Color, m_cpBrush.GetAddressOf());
	assert(SUCCEEDED(result) && "�e�L�X�g�̐F���s");

	// �e�p�̃u���V���쐬
	result = m_cpRenderTarget->CreateSolidColorBrush(m_Setting.shadowColor, m_cpShadowBrush.GetAddressOf());
	assert(SUCCEEDED(result) && "�e�p�̃u���V���쐬���s");

	//return result;
}

void DirectWrite::SetFonts(FontList::FontName _fontname, DWRITE_FONT_WEIGHT _fontWeight,
	DWRITE_FONT_STYLE _fontStyle, DWRITE_FONT_STRETCH _fontStretch, FLOAT _fontSize,
	std::wstring _localeName, DWRITE_TEXT_ALIGNMENT _textAlignment, D2D1_COLOR_F _Color,
	D2D1_COLOR_F _shadowColor, D2D1_POINT_2F _shadowOffset)
{
	FontData f;
	f.font = _fontname;
	f.fontWeight = _fontWeight;
	f.fontStyle = _fontStyle;
	f.fontStretch = _fontStretch;
	f.fontSize = _fontSize;
	f.localeName = _localeName;
	f.textAlignment = _textAlignment;
	f.Color = _Color;
	f.shadowColor = _shadowColor;
	f.shadowOffset = _shadowOffset;

	SetFont(f);
}

HRESULT DirectWrite::DrawString(std::wstring _str, DirectX::SimpleMath::Vector2 _pos,
	D2D1_DRAW_TEXT_OPTIONS _options, bool _shadow)
{
	HRESULT result = S_OK;

	// ������̕ϊ�
	//std::wstring wstr = StringToWString(_str);
	std::wstring wstr = _str;

	// �w�肵�����������s�ɒu����������������쐬
	for (size_t pos = wstr.find(L'��'); pos != std::wstring::npos; pos = wstr.find(L'��', pos + 1))
	{
		wstr.replace(pos, 1, L"\n");
	}

	// �^�[�Q�b�g�T�C�Y�̎擾
	D2D1_SIZE_F targetSize = m_cpRenderTarget->GetSize();

	// �e�L�X�g���C�A�E�g���쐬
	if (!m_cpTextLayout || m_CurrentText != wstr)
	{
		m_CurrentText = wstr; // ���݂̃e�L�X�g���X�V

		// �ȑO�̃e�L�X�g���C�A�E�g�����
		m_cpTextLayout.Reset();

		result = m_cpDWriteFactory->CreateTextLayout(wstr.c_str(),
			static_cast<UINT32>(wstr.size()), m_cpTextFormat.Get(), targetSize.width,
			targetSize.height, m_cpTextLayout.GetAddressOf());
		assert(SUCCEEDED(result) && "�e�L�X�g���C�A�E�g�쐬���s");
	}

	// �`��ʒu�̊m��
	D2D1_POINT_2F points;
	points.x = _pos.x;
	points.y = _pos.y;

	// �`��̊J�n
	m_cpRenderTarget->BeginDraw();

	// �e��`�悷��ꍇ
	if (_shadow)
	{
		// �e�̕`��
		m_cpRenderTarget->DrawTextLayout(D2D1::Point2F(points.x - m_Setting.shadowOffset.x,
			points.y - m_Setting.shadowOffset.y),
			m_cpTextLayout.Get(),
			m_cpShadowBrush.Get(),
			_options);
	}

	// �`�揈��
	m_cpRenderTarget->DrawTextLayout(points, m_cpTextLayout.Get(), m_cpBrush.Get(), _options);

	// �`��̏I��
	result = m_cpRenderTarget->EndDraw();
	assert(SUCCEEDED(result) && "�`��̏I���������s");

	return S_OK;
}

HRESULT DirectWrite::DrawString(std::string _str, D2D1_RECT_F _rect, D2D1_DRAW_TEXT_OPTIONS _options, bool _shadow)
{
	HRESULT result = S_OK;

	// ������̕ϊ�
	std::wstring wstr = StringToWString(_str.c_str());

	// �`��̊J�n
	m_cpRenderTarget->BeginDraw();

	if (_shadow)
	{
		// �e�̕`��
		m_cpRenderTarget->DrawText(wstr.c_str(),
			(UINT32)wstr.size(),
			m_cpTextFormat.Get(),
			D2D1::RectF(_rect.left - m_Setting.shadowOffset.x,
				_rect.top - m_Setting.shadowOffset.y,
				_rect.right - m_Setting.shadowOffset.x,
				_rect.bottom - m_Setting.shadowOffset.y),
			m_cpShadowBrush.Get(), _options);
	}

	// �`�揈��
	m_cpRenderTarget->DrawText(wstr.c_str(), (UINT32)wstr.size(), m_cpTextFormat.Get(),
		_rect, m_cpBrush.Get(), _options);

	// �`��̏I��
	result = m_cpRenderTarget->EndDraw();
	assert(SUCCEEDED(result) && "�`��̏I���������s");


	return S_OK;
}

HRESULT DirectWrite::FontLoader()
{
	HRESULT result = S_OK;

	// �z�񂩂�std::vector�֕ϊ�
	std::vector<std::wstring> fontPaths(std::begin(FontPath), std::end(FontPath));

	result = LoadFontFiles(fontPaths);

	// �J�X�^���t�H���g�R���N�V�����̍쐬
	result = m_cpDWriteFactory->CreateCustomFontCollection(
		pFontCollectionLoader.Get(),
		m_cpFontFileList.data(),
		static_cast<UINT32>(m_cpFontFileList.size()),
		&fontCollection
	);
	if (FAILED(result)) { return result; }

	// �t�H���g�����擾
	result = GetFontFamilyName(fontCollection.Get(), L"en-us"); // ���P�[���͓K�؂Ȓl�ɐݒ肵�Ă�������
	if (FAILED(result)) { return result; }

	return S_OK;
}


std::wstring DirectWrite::GetFontName(int _num)
{
	// �t�H���g���̃��X�g���󂾂����ꍇ
	if (m_FontNamesList.empty())
	{
		return std::wstring();
	}

	// ���X�g�̃T�C�Y�𒴂��Ă����ꍇ
	if (_num >= static_cast<int>(m_FontNamesList.size()))
	{
		return m_FontNamesList[0];
	}

	return m_FontNamesList[_num];
}

int DirectWrite::GetFontNameNum()
{
	return (int)m_FontNamesList.size();
}

HRESULT DirectWrite::GetFontFamilyName(IDWriteFontCollection* _customFontCollection, std::wstring _locale)
{
	HRESULT result = S_OK;

	// null �`�F�b�N
	if (!_customFontCollection)
	{
		return E_POINTER; // �����ȃ|�C���^�̏ꍇ�̓G���[��Ԃ�
	}

	// �t�H���g�t�@�~���[���ꗗ�����Z�b�g
	m_FontNamesList.clear();

	// �t�H���g�̐����擾
	UINT32 familyCount = _customFontCollection->GetFontFamilyCount();
	if (familyCount == 0)
	{
		return S_FALSE; // �t�H���g�t�@�~���[�����݂��Ȃ��ꍇ�A�G���[�ł͂Ȃ��x���Ƃ��Ĉ���
	}

	for (UINT32 i = 0; i < familyCount; ++i)
	{
		// �t�H���g�t�@�~���[�̎擾
		ComPtr<IDWriteFontFamily> fontFamily = nullptr;
		result = _customFontCollection->GetFontFamily(i, fontFamily.GetAddressOf());
		assert(SUCCEEDED(result) && "�t�H���g�t�@�~���[�̎擾���s");

		// �t�H���g�t�@�~���[���̈ꗗ���擾
		ComPtr<IDWriteLocalizedStrings> familyNames = nullptr;
		result = fontFamily->GetFamilyNames(familyNames.GetAddressOf());
		assert(SUCCEEDED(result) && "�t�H���g�t�@�~���[���̈ꗗ�擾���s");

		// �w�肳�ꂽ���P�[���ɑΉ�����C���f�b�N�X������
		UINT32 index = 0;
		BOOL exists = FALSE;
		result = familyNames->FindLocaleName(_locale.c_str(), &index, &exists);
		assert(SUCCEEDED(result) && "�w�肳�ꂽ���P�[���ɑΉ�����C���f�b�N�X���������s");

		// �w�肳�ꂽ���P�[����������Ȃ������ꍇ�A�f�t�H���g�̃��P�[�����g�p
		if (!exists)
		{
			result = familyNames->FindLocaleName(L"en-us", &index, &exists);
			assert(SUCCEEDED(result) && "�f�t�H���g���P�[���ɑΉ�����C���f�b�N�X���������s");

			if (!exists)
			{
				continue; // �f�t�H���g���P�[���ł�������Ȃ������ꍇ�̓X�L�b�v
			}
		}

		// �t�H���g�t�@�~���[���̒������擾
		UINT32 length = 0;
		result = familyNames->GetStringLength(index, &length);
		assert(SUCCEEDED(result) && "�t�H���g�t�@�~���[���̒������擾���s");

		// �t�H���g�t�@�~���[���̎擾
		std::wstring fontName(length + 1, L'\0'); // �I�[�������܂߂��o�b�t�@���m��
		result = familyNames->GetString(index, &fontName[0], length + 1);
		assert(SUCCEEDED(result) && "�t�H���g�t�@�~���[���̎擾���s");

		// �t�H���g�t�@�~���[����ǉ�
		m_FontNamesList.push_back(fontName);
	}

	return result;
}


HRESULT DirectWrite::GetAllFontFamilyName(IDWriteFontCollection* _customFontCollection)
{
	HRESULT result = S_OK;

	// �t�H���g�t�@�~���[���ꗗ�����Z�b�g
	m_FontNamesList.clear();

	// �t�H���g�t�@�~���[�̐����擾
	UINT32 familyCount = _customFontCollection->GetFontFamilyCount();

	for (UINT32 i = 0; i < familyCount; ++i)
	{
		// �t�H���g�t�@�~���[�̎擾
		ComPtr<IDWriteFontFamily> fontFamily = nullptr;
		result = _customFontCollection->GetFontFamily(i, fontFamily.GetAddressOf());
		assert(SUCCEEDED(result) && "�t�H���g�t�@�~���[�̎擾���s");

		// �t�H���g�t�@�~���[���̈ꗗ���擾
		ComPtr<IDWriteLocalizedStrings> familyNames = nullptr;
		result = fontFamily->GetFamilyNames(familyNames.GetAddressOf());
		assert(SUCCEEDED(result) && "�t�H���g�t�@�~���[���̈ꗗ�擾���s");

		// �t�H���g�t�@�~���[���̐����擾
		UINT32 nameCount = familyNames->GetCount();

		// �t�H���g�t�@�~���[���̐������J��Ԃ�
		for (UINT32 j = 0; j < nameCount; ++j)
		{
			// �t�H���g�t�@�~���[���̒������擾
			UINT32 length = 0;
			result = familyNames->GetStringLength(j, &length);
			assert(SUCCEEDED(result) && "�t�H���g�t�@�~���[���̒������擾���s");

			// std::wstring ���g�p���ĕ�������i�[
			std::wstring fontName(length + 1, L'\0');  // �I�[�������܂߂��o�b�t�@���m��
			result = familyNames->GetString(j, &fontName[0], length + 1);
			assert(SUCCEEDED(result) && "�t�H���g�t�@�~���[���̎擾���s");

			// �t�H���g�t�@�~���[����ǉ�
			m_FontNamesList.push_back(fontName);
		}
	}

	return result;
}

HRESULT DirectWrite::LoadFontFiles(const std::vector<std::wstring>& fontPaths)
{
	HRESULT result = S_OK;

	for (const auto& path : fontPaths)
	{
		ComPtr<IDWriteFontFile> fontFile;
		result = m_cpDWriteFactory->CreateFontFileReference(path.c_str(), nullptr, fontFile.GetAddressOf());
		if (FAILED(result)) {
			wprintf(L"Failed to load font file: %s\n", path.c_str());
			return result;
		}

		// �t�H���g�t�@�C�������X�g�ɒǉ�
		m_cpFontFileList.push_back(fontFile);
	}

	return result;
}

HRESULT DirectWrite::DrawStringPartial(std::wstring _str, DirectX::SimpleMath::Vector2 _pos,
	bool _shadow, bool _changeText, float _delaytime,float _elapsedTime,
	D2D1_DRAW_TEXT_OPTIONS _options)
{
	HRESULT result = S_OK;

	// ������̕ϊ�
	//std::wstring wstr = StringToWString(_str);
	std::wstring wstr = _str;

	// �o�ߎ��ԂɊ�Â��ĕ\�����镶�������v�Z
	m_TotalTime += _elapsedTime;

	// �ꕶ�����Ƃɒx��������
	size_t numCharsToDisplay = static_cast<size_t>(m_TotalTime / _delaytime);

	// ���������\�����镶����̒����𒴂��Ȃ��悤�ɐ���
	if (numCharsToDisplay > wstr.size()) {
		numCharsToDisplay = wstr.size();
	}

	// �\�����镔����������擾
	std::wstring partialStr = wstr.substr(0, numCharsToDisplay);

	// �w�肵�����������s�ɒu����������������쐬
	for (size_t pos = partialStr.find(L'��'); pos != std::wstring::npos; pos = partialStr.find(L'��', pos + 1))
	{
		partialStr.replace(pos, 1, L"\n");
	}

	// �e�L�X�g���C�A�E�g���Đ�������K�v������ꍇ�̂ݐ���
	if (!m_cpTextLayout || m_CurrentText != partialStr)
	{
		m_CurrentText = partialStr; // ���݂̃e�L�X�g���X�V

		// �ȑO�̃e�L�X�g���C�A�E�g�����
		m_cpTextLayout.Reset();

		// �V�����e�L�X�g���C�A�E�g���쐬
		result = m_cpDWriteFactory->CreateTextLayout(partialStr.c_str(),
			static_cast<UINT32>(partialStr.size()), m_cpTextFormat.Get(), m_cpRenderTarget->GetSize().width,
			m_cpRenderTarget->GetSize().height, m_cpTextLayout.GetAddressOf());
		assert(SUCCEEDED(result) && "�e�L�X�g���C�A�E�g�쐬���s");

		if (partialStr.size() >= 4 && _changeText)
		{
			ComPtr<ID2D1SolidColorBrush> colorBrush;
			result = ChangeTextFont(m_cpTextLayout.Get(),FontList::FontName::MelodyLine, 4, 6);
			assert(SUCCEEDED(result) && "�t�H���g�̐ݒ莸�s");
			m_cpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow),
				colorBrush.GetAddressOf());
			m_cpTextLayout->SetDrawingEffect(colorBrush.Get(), {4,5});
		}
		
	}

	// �`��ʒu�̊m��
	D2D1_POINT_2F points = { _pos.x, _pos.y };

	// �`��̊J�n
	m_cpRenderTarget->BeginDraw();

	// �e��`�悷��ꍇ
	if (_shadow)
	{
		// �e�̕`��
		m_cpRenderTarget->DrawTextLayout(D2D1::Point2F(points.x - m_Setting.shadowOffset.x,
			points.y - m_Setting.shadowOffset.y),
			m_cpTextLayout.Get(),
			m_cpShadowBrush.Get(),
			_options);
	}

	// �e�L�X�g�̕`��
	m_cpRenderTarget->DrawTextLayout(points, m_cpTextLayout.Get(), m_cpBrush.Get(), _options);

	// �`��̏I��
	result = m_cpRenderTarget->EndDraw();
	assert(SUCCEEDED(result) && "�`��̏I���������s");

	return S_OK;
}

void DirectWrite::ResetText()
{
	m_cpTextLayout.Reset();
	m_CurrentText.clear();
	m_TotalTime = 0.0f;
}

//void DirectWrite::ReleaseFontCollectionLoader()
//{
//	m_cpDWriteFactory->UnregisterFontCollectionLoader(pFontCollectionLoader.Get());
//	pFontCollectionLoader.Reset();
//}

DirectWrite::~DirectWrite()
{
	// �t�H���g�R���N�V�������[�_�[�̓o�^����
	if (m_cpDWriteFactory && pFontCollectionLoader)
	{
		m_cpDWriteFactory->UnregisterFontCollectionLoader(pFontCollectionLoader.Get());
	}

	// �ÓI�����o�[�̉��
	if (pFontCollectionLoader)
	{
		pFontCollectionLoader.Reset(); // COM �|�C���^�����Z�b�g
	}

	// �ǉ��̃N���[���A�b�v
	m_cpFontFileList.clear();

	// �e���\�[�X�̉��
	m_cpD2DFactory.Reset();
	m_cpRenderTarget.Reset();
	m_cpBrush.Reset();
	m_cpShadowBrush.Reset();
	m_cpDWriteFactory.Reset();
	m_cpTextFormat.Reset();
	m_cpTextLayout.Reset();
	m_cpBackBuffer.Reset();

	// �t�H���g�R���N�V�����̃��Z�b�g
	fontCollection.Reset();
}