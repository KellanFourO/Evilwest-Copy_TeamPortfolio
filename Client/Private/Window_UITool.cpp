#include "stdafx.h"
#include "Window_UITool.h"
#include "UI_Anything.h"

/* error �ܺ����� ��ȣ : define �ɾ������ */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CWindow_UITool::CWindow_UITool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImgui_Window(pDevice, pContext)
{
}


HRESULT CWindow_UITool::Initialize()
{
	//! ����� Ư���� ��ɾ���. ���� �ʿ��� �� ���Ƽ� ����.
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	/* Texture ������ */


	//UI_TextureLoad();

	//LoadImg(ConverCtoWC(ConverWStringtoC(TEXT("../Bin/Resources/Textures/UI/Textures"))));
	LoadImg(ConverCtoWC(ConverWStringtoC(TEXT("../Bin/Resources/Textures/UI/Textures/PlayerHUD"))));

	// �̹��� �ε� Test
	_int iSize = m_vecPaths.size();
	for (_int i = 0; i < iSize; i++)
	{
		IMAGEINFO* tTexture = new IMAGEINFO;

		_bool bRet = LoadTextureFromFile(ConverWStringtoC(m_vecPaths[i]->strFilePath), &tTexture->SRV_Texture, &tTexture->iImage_Width, &tTexture->iImage_Height);
		IM_ASSERT(bRet);

		/* Texture ũ�⸦ ���Ƿ� �����ϰ� �ʹٸ�, ���⼭ ������ �������. ���� ���ָ� ���� �ؽ�ó ũ���� ���´�. �� �� ���� ũ�⸦ �̿��� ������ �ٿ��� ����ص� ���� �� ����. */
		//tTexture->iImage_Width = 100;
		//tTexture->iImage_Height = 100;
		tTexture->iTextureNum = i;
		m_vecTexture.push_back(tTexture);

	}

	// ���ڿ� ���͸� const char* �迭�� ��ȯ
	//std::vector<const char*> charImagePaths;
	for (auto& iter : m_vecPaths)
	{
		m_vecImagePaths.push_back(ConverWStringtoC(iter->strFilePath.c_str()));
	}

	return S_OK;
}

void CWindow_UITool::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	/* Test */
	// ������ â ũ�� ����� ����� ���� ���콺��ġ���� �̿��� â ����� �ٲ���� �� �� ����.
	//ImGui::IsMouseDown();
	//m_tImGuiDESC.vWindowSize; 

	__super::Begin();
	ImGui::Text("UI_Tool");

	// Test
	//UI_List(fTimeDelta);


	UI2D_Setting(fTimeDelta);

	/* �̹��� ���� �� �̸����� */
	ImagePreview(fTimeDelta);

	ImGui::Dummy(ImVec2(0, 5.f)); // ����

	/* Default : LastNumber */
	UI_ToolTip(fTimeDelta); // Tip : ������ �������̽� ��� ����� ���� �������� ȣ��Ǿ���Ѵ�. (�ȱ׷��� �ٸ� �༮���� ������ �з��� ��µ�)
	__super::End();
}

void CWindow_UITool::Render()
{

}

void CWindow_UITool::UI_List(_float fTimeDelta)
{
	/* Test Value */
	m_tUI_Info.strName = "Test UI List";
	m_tUI_Info.iNum = 1;
	m_tUI_Info.fNum = 1.5f;

	// ������ ������� ǥ��
	if (ImGui::TreeNode(m_tUI_Info.strName.c_str())) {
		ImGui::Text(u8"��1 : %d", m_tUI_Info.iNum);
		ImGui::Text(u8"��2 : %.2f", m_tUI_Info.fNum);

		// ��ư
		if (ImGui::Button(u8"��ư"))
		{

		}

		ImGui::TreePop();
	}
}

void CWindow_UITool::UI_ToolTip(_float fTimeDelta)
{
	/* �׽�Ʈ �� */
	_int	iTestNum = 1;
	_float	fTestValue1 = 33.f;
	_float	fTestValue2 = 44.f;

	/* ���콺 ������ ��ġ */
	_float	fTestX = 50.f;
	_float	fTestY = 500.f;

	/* ���콺 ������ ���� */
	_float	fHoverRangeX = 200.f;
	_float	fHoverRangeY = 200.f;

	/* ���콺 ������ ������ ����â�� ũ�� */
	_float	fScreenPosX = 20.f;
	_float	fScreenPosY = 40.f;

	//// ���콺�� �ش� ��ġ ���� �ִ� ��쿡�� ���� ǥ�� (���콺 ����)
	//if (ImGui::IsMouseHoveringRect(
	//	ImVec2(fTestX - fHoverRangeX, fTestY - fHoverRangeY),
	//	ImVec2(fTestX + fHoverRangeX, fTestY + fHoverRangeY)))
	//{

	//	// ǥ���� ����
	//	ImGui::SetCursorScreenPos(ImVec2(fTestX + fScreenPosX, fTestY - fScreenPosY));
	//	ImGui::BeginTooltip();

	//	ImGui::EndTooltip(); // ���� ����
	//}
}

// �̹����� �޸𸮿� �ε��ϴ� �Լ�
std::vector<unsigned char> CWindow_UITool::UI_LoadImage(const std::string& filename, int& width, int& height, int& channels)
{
	// ���� ���� �б� (�̸������ �ִ� 100����Ʈ������ ���)
	std::ifstream file(filename, std::ios::binary);
	if (file.is_open()) {
		char buffer[101]; // 100����Ʈ + �� ����
		if (file.read(buffer, 100)) {
			buffer[file.gcount()] = '\0'; // �� ���� �߰�
			std::cout << "Preview: " << buffer << std::endl;
		}
		file.close();
	}
	else {
		std::cerr << "Failed to open file: " << filename << std::endl;
	}

	// ���� ũ�� Ȯ��
	file.seekg(0, std::ios::end);
	std::streamsize fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// ���� ������ ������ ���� ����
	std::vector<unsigned char> imageData(fileSize);
	if (!file.read(reinterpret_cast<char*>(imageData.data()), fileSize)) {
		std::cerr << "Failed to read file: " << filename << std::endl;
		return {};
	}

	// �̹��� �ʺ�, ����, ä�� �� ���� (���÷� ���� �� ���)
	width = 256;
	height = 256;
	channels = 4; // RGBA

	return imageData;
}

// Simple helper function to load an image into a DX11 texture with common settings
bool CWindow_UITool::LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	m_pDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	m_pDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}

// std::string ���͸� const char* �迭�� ��ȯ�ϴ� �Լ�
std::vector<const char*> CWindow_UITool::ConvertStringVectorToCharArray(const std::vector<std::string>& stringVector)
{
	std::vector<const char*> charArray;
	charArray.reserve(stringVector.size());
	for (const std::string& str : stringVector)
	{
		charArray.push_back(str.c_str());
	}
	return charArray;
}

std::wstring CWindow_UITool::ConvertToWideString(const std::string& ansiString)
{
	int wideStrLen = MultiByteToWideChar(CP_ACP, 0, ansiString.c_str(), -1, nullptr, 0);
	if (wideStrLen == 0)
	{
		// ��ȯ ���� ó��
		return L"Fail";
	}

	std::wstring wideString(wideStrLen, L'\0');
	MultiByteToWideChar(CP_ACP, 0, ansiString.c_str(), -1, &wideString[0], wideStrLen);

	return wideString;
}

char* CWindow_UITool::ConverWStringtoC(const wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	char* result = new char[size_needed];
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, result, size_needed, NULL, NULL);
	return result;
}

wchar_t* CWindow_UITool::ConverCtoWC(char* str)
{
	_tchar* pStr;
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
	pStr = new WCHAR[strSize];
	MultiByteToWideChar(CP_ACP, 0, str, (_int)strlen(str) + (size_t)1, pStr, strSize);

	return pStr;
}

void CWindow_UITool::LoadImg(const _tchar* folderPath)
{
	// ã�� �̹��� �����͸� ���� ����
	WIN32_FIND_DATA findData;

	// ���� ���
	wstring wfolderPath = (wstring)folderPath + L"/*.*";

	// ���� ��� ���� �� �ڵ� ��ȯ
	HANDLE hFind = FindFirstFile(wfolderPath.c_str(), &findData);

	// �ڵ��� ���� ���
	if (hFind != INVALID_HANDLE_VALUE)
	{
		// ���� ���丮
		vector<wstring> subDirectories;
		// �̹��� ����
		vector<wstring> imageFiles;
		do
		{
			//  ������ �Ӽ� ������ �Էµȴ�. (���丮���� �������� ��)
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (lstrcmp(findData.cFileName, L".") != 0 && lstrcmp(findData.cFileName, L"..") != 0)
				{
					// ���� ��� = ��� + / + ���ϸ�
					wstring subFolderPath = (wstring)folderPath + L"/" + findData.cFileName;
					subDirectories.push_back(subFolderPath);
					LoadImg(subFolderPath.c_str());
				}
			}
			else
			{
				// ������ ���, �̹��� �������� Ȯ���ϰ� �ε�
				wstring filePath = (wstring)folderPath + L"/" + findData.cFileName;

				// ���� Ȯ���� Ȯ��
				if (wcsstr(findData.cFileName, L".png") || wcsstr(findData.cFileName, L".jpg") ||
					/*wcsstr(findData.cFileName, L".bmp") || wcsstr(findData.cFileName, L".tga") ||*/
					wcsstr(findData.cFileName, L".dds"))
				{
					PATHINFO* pPathInfo = new PATHINFO;

					pPathInfo->strFilePath = filePath;
					pPathInfo->cFileName[MAX_PATH] = *findData.cFileName;
					pPathInfo->iPathNum = m_iTextureNum;

					m_vecPaths.push_back(pPathInfo);

					m_iTextureNum++;
				}
			}
		} while (FindNextFile(hFind, &findData));

		
		FindClose(hFind);
	}
}

void CWindow_UITool::ImagePreview(_float fTimeDelta)
{
	if (ImGui::CollapsingHeader(u8"�̹���"))
	{
		if (ImGui::ListBox("Image Paths", &m_iSelectedPathIndex, m_vecImagePaths.data(), (int)m_vecImagePaths.size()))
		{

		}
	}

	//ImGui::Text("pointer = %p", m_vecTexture[m_iSelectedPathIndex]->SRV_Texture);
	//ImGui::Text("size = %f x %f", m_vecTexture[m_iSelectedPathIndex]->iImage_Width, m_vecTexture[m_iSelectedPathIndex]->iImage_Height);
	ImGui::Image((void*)m_vecTexture[m_iSelectedPathIndex]->SRV_Texture, ImVec2(m_vecTexture[m_iSelectedPathIndex]->iImage_Width, m_vecTexture[m_iSelectedPathIndex]->iImage_Height));

}

void CWindow_UITool::UI2D_Setting(_float fTimeDelta)
{
	ImGui::CollapsingHeader("2D_Setting");
	/* Scale */
	ImGui::SeparatorText(u8"ũ�� ����");
	ImGui::InputFloat2("Scale", m_fScale);


	/* Position*/
	ImGui::SeparatorText(u8"��ġ ����");
	ImGui::InputFloat2("Position", m_fPosition);

	ImGui::Dummy(ImVec2(0, 2.5)); // ����
	ImGui::Separator();


	if (ImGui::Button("Create"))
	{
		UI2D_Create(fTimeDelta);
	}

	ImGui::SameLine(70.f);

	if (ImGui::Button("Delete"))
	{
		UI2D_Delete(fTimeDelta);
	}

}

void CWindow_UITool::UI2D_Create(_float fTimeDelta)
{

}

void CWindow_UITool::UI2D_Delete(_float fTimeDelta)
{

}

// ImGui�� ����Ͽ� �̹����� ǥ���ϴ� �Լ�
void CWindow_UITool::ShowImagePreview(const std::vector<unsigned char>& imageData, int width, int height)
{
	ImVec2 imageSize(width, height);
	ImGui::Image(reinterpret_cast<void*>(const_cast<unsigned char*>(imageData.data())), imageSize);
}

CWindow_UITool* CWindow_UITool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWindow_UITool* pInstance = new CWindow_UITool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CWindow_UITool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWindow_UITool::Free()
{
	__super::Free();
}