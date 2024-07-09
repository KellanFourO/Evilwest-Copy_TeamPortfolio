# Evilwest 모작 팀 프로젝트

- **기술**:
    ![C](https://img.shields.io/badge/C%20-%232370ED.svg?&logo=c&logoColor=white)
    ![C++](https://img.shields.io/badge/C++%20-%2300599C.svg?&logo=c%2B%2B&logoColor=white)
    ![Direct 11](https://img.shields.io/badge/Direct%2011%20-%236DA252.svg?&logo=Direct11&logoColor=white)
    ![HLSL](https://img.shields.io/badge/HLSL%20-%236DA252.svg?&logo=HLSL&logoColor=white)
    ![Imgui](https://img.shields.io/badge/Imgui%20-%23F94877.svg?&logo=Imgui&logoColor=white)

<!--프로젝트 대문 -->
[![Video Label](https://ifh.cc/g/mTRklo.jpg)](https://youtu.be/E9E7zZkeh3M?t=652)

# 개발 기간 및 참여 인원
   
 개발기간 : 2024.02.13 ~ 2024.04.15
   
 참여 인원 : 6 people

# 중점 구현 사항

프로젝트에서 제가 맡은 파트는 맵 / 서브 프레임 워크 / 기믹 / 사운드 였습니다.

- 인스턴싱 기법을 통해 환경 오브젝트들을 최적화 하였습니다.

- 맵에 사용되는 모델 리소스의 개수가 180개였고, 이에 편의성과 레벨의 필요한 리소스만 로드 하기위해 특정 디렉토리 경로안의 모든 폴더를 폴더명으로 분류하여 애니메이션의 유무나 오브젝트의 타입(상호작용 등)을 판단하여 로드하였습니다.

- 맵 툴에서 몬스터 트리거와 UI 트리거를 배치하여 몬스터생성, UI 출력 등을 하였고, 트리거를 밟았을 때 스폰되는 몬스터와 UI의 정보는 로딩 레벨에서 전부 로드 시켜놓아 순간적인 프레임 부하를 최소화 하였습니다.

- 맵에 플레이어가 지나갈 수 없는 지역을 기믹을 해결해서 진행할 수 있도록 맵 툴에서 여러가지 타입의 기믹 오브젝트를 배치할 수 있도록 구성하였고, 배치하였습니다.

-  기믹 오브젝트를 상속받는 자식 기믹 오브젝트들은 가상 함수 오버라이딩을 통해, 기능을 분류하여 구현 및 관리했고 기본 베이스는 네비게이션 메시와 충돌체를 활용하여 플레이어와 상호작용할 수 있도록 제작 되었습니다.
  또한 그룹화 기능을 부여하여 특정 기믹 오브젝트가 활성화 될 경우, 같은 그룹에 속한 다른 오브젝트들에게 정보를 전달할 수 있도록 하였고, 자신의 기능에 맞는 행동을 하도록 구현했습니다.
  로프 액션 기믹, 낙법 기믹 등 플레이어와 오브젝트가 상호작용 시에 맵 높낮이, 거리 등 상황에 맞는 액션 처리를 할 수 있도록 기믹 오브젝트들은 플레이어의 루트모션 값을 보간시킬 수 있는 속성 값을 가질 수 있도록 하였습니다.

- 툴 프레임워크를 작성하여 각각 다른 파트를 맡은 팀원들이 모두 툴상에서 분리하여 작업할 수 있도록 하였고, 또한 다른 툴의 기능이 필요할 시에는 가져다 쓸 수 있도록 편의성을 우선시하여 제작하였습니다.
  Imgui 라이브러리를 메인으로 사용하였고, imguizmo, imguifiledialog 등 편의성 라이브러리를 추가하여 오브젝트 배치, 저장 및 불러오기 기능을 구현했습니다.

- 맵 툴에서 몬스터 또는 보스, 트리거 , 조명 , 1인칭과 3인칭 카메라의 전환 , 레이 캐스트를 통한 픽킹 처리, 네비게이션 메시 등 게임을 제작할 때 필요한 기본적인 요소들을 구현 및 실사용하였습니다.


