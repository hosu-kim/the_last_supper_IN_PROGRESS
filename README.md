```bash
the_oracle/
├── LICENSE                # 프로젝트의 라이선스 파일
├── Makefile               # 빌드 스크립트
├── README.md              # 프로젝트 설명 및 사용법 안내
├── include/               # 헤더 파일 디렉토리
│   ├── philosophers.h     # 철학자 관련 구조체, 함수 선언
│   ├── simulation.h       # 시뮬레이션 관리용 함수 선언
│   └── utils.h            # 공통 유틸리티 함수 선언
└── src/                   # 소스 코드 디렉토리
    ├── main.c             # 프로그램 실행의 진입점
    ├── philosophers.c     # 철학자 동작 구현
    ├── simulation.c       # 시뮬레이션 로직 구현
    └── utils.c            # 공통 유틸리티 함수 구현
```