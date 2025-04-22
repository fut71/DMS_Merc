# 돈미세 용병단(DMS Mercenery)
### 현재 버전: v1.0.1 (Build 24.1111)

<img src="dms_title.png" alt="DMS Mercenery 타이틀 로고" width="500">
동영상 링크 : https://www.youtube.com/watch?v=CUTfOIyOGYs

---

## 개요 Overview
 - 돈미세 용병단은 Direct3D 11를 사용하여 개발된 게임 엔진을 기반으로 한 게임입니다. 현재 리포지토리에서는 리소스와 실행 파일등을 제외하고, 소스 코드와 헤더만을 제공합니다.
The DMS Mercenery is a game built on a custom engine developed using Direct3D 11. Currently, the repository only provides source code and headers, excluding resources and executables.
---

## 설치 방법 Installation Instructions
1. 압축 파일을 다운로드하고 원하는 디렉터리에 압축을 해제합니다.
	Download and extract the compressed file to your desired directory.

2. dms.bat 를 실행하여 바로가기 파일을 생성합니다.
	Run dms.bat to create a shortcut file.

3. 바로가기 파일을 클릭하여 게임을 시작합니다.
	Click the shortcut to start the game.
---

## 제거 방법 Uninstallation Instructions
0. 작업 관리자의 백그라운드 프로세스에서 DSM_Merc.exe 작업 끝내기합니다.
	End the DSM_Merc.exe background process via Task Manager.

1. 설치된 디렉터리에서 게임 폴더를 삭제합니다.
	Delete the game folder from the installation directory.

2. Windows 설정 > 프로그램 추가/제거에서 해당 프로그램을 확인 후 제거합니다. (미구현)
	Check Windows Settings > Add or Remove Programs for the program and uninstall. (Currently not implemented)

---

## 시스템 요구사항 System Requirements

### 최소 요구사항 Minimum Requirements
- **OS**: Windows 10 (64-bit)
- **CPU**: Intel Core i3 or AMD equivalent
- **GPU**: DirectX 11 compatible GPU (4GB VRAM or higher)
- **RAM**: 4GB or more
- **Storage**: 2GB free space

### 권장 요구사항 Recommended Requirements
- **OS**: Windows 10/11 (64-bit)
- **CPU**: Intel Core i5 or AMD Ryzen 5 or higher
- **GPU**: DirectX 11 compatible GPU (6GB VRAM or higher)
- **RAM**: 8GB or more
- **Storage**: 2GB free space

### 소프트웨어 구성요소 Software Components
- DirectX 11
- Sound: FMOD
- Input: Keyboard, mouse support
- Physx
- Entt
- Spdlog
- Imgui
---

## 조작법 Controls
- **상호작용**: 마우스 좌클릭
	Interaction: Left Mouse Click
- **팝업창 ON/OFF**: ESC 키
	Popup Window ON/OFF: ESC Key

---

## 버전 기록 History

### v1.0.0
- 첫 공식 릴리즈 (Build 241111)
	Official Initial Release (Build 241111)
### v1.0.1
- 마우스 드래그를 막아서 장애물에 동전 배치되는 버그 발생 빈도를 줄임
	Reduced bug occurrence rate for coin placement on obstacles by disabling mouse dragging.
