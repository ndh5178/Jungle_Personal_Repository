# AGENTS.md

이 문서는 AI 에이전트가 이 저장소에서 작업할 때 반드시 따라야 할 기준을 정의합니다.

## 1. 제1원칙

아래 항목은 절대적으로 지켜져야 합니다.

### .gitignore

- `AGENTS.md`를 제외한 모든 Markdown 파일은 `.gitignore`에 포함합니다.
- 테스트 및 구현 산출물은 `.gitignore`에 포함합니다.
- 빌드 결과, 디스크 이미지, 로그, 제출 아카이브 등 생성 파일은 특별한 이유가 없으면 Git 추적 대상에 포함하지 않습니다.

### 일반

- LF 줄바꿈 규칙을 유지하고, 저장소의 `.gitattributes` 설정을 따릅니다.
- 디렉터리와 repository에 위험을 초래할 수 있는 명령어는 금지합니다.
- 위험한 명령이 꼭 필요한 경우, 사용자가 영향 범위를 명확히 인지할 수 있도록 먼저 승인 요청을 해야 합니다.
- 사용자 요청 없이 `git reset --hard`, 강제 push, 무단 브랜치 정리, 기존 작업 되돌리기, 대량 삭제를 수행하지 않습니다.
- 모든 빌드와 테스트는 DevContainer 내부 Ubuntu 22.04 x86-64 환경에서 수행합니다.
- 호스트 환경에서 Pintos를 직접 빌드하거나 테스트하지 않습니다.
- Pintos 관련 명령을 실행하기 전에는 `source pintos/activate`를 먼저 수행합니다.

## 2. 아키텍처 구성

이 저장소는 KAIST Pintos 기반 운영체제 과제 중 Week11 VM 작업을 위한 코드베이스입니다.

Pintos는 단계별로 누적되는 구조입니다. `threads`의 스케줄링과 동기화, `userprog`의 프로세스/시스템 콜 구현이 `vm` 테스트 결과에 직접 영향을 줄 수 있습니다.

주요 구성은 다음과 같습니다.

- `threads`: 커널 스레드, 스케줄러, 동기화, 인터럽트 처리
- `userprog`: 사용자 프로세스 실행, 예외 처리, 시스템 콜
- `vm`: 가상 메모리, 페이지 폴트, 프레임, 스왑, mmap
- `filesys`: 파일시스템, 디렉터리, 버퍼 캐시
- `devices`: 타이머, 디스크, 키보드, 시리얼, VGA
- `lib`: 커널/유저 공용 라이브러리와 자료구조
- `tests`: 프로젝트별 테스트 케이스
- `utils`: Pintos 실행과 보조 유틸리티

## 3. 폴더 구조

```text
.
├─ .devcontainer/        # VSCode DevContainer 개발 환경 설정
├─ .vscode/              # VSCode 보조 설정
├─ pintos/
│  ├─ threads/           # 스레드, 스케줄러, 동기화, 인터럽트
│  ├─ userprog/          # 사용자 프로세스, 예외 처리, 시스템 콜
│  ├─ vm/                # 가상 메모리, 프레임, 스왑, 페이지 관리
│  ├─ filesys/           # 파일시스템, 디렉터리, 캐시
│  ├─ devices/           # 타이머, 디스크, 키보드, 시리얼, VGA
│  ├─ include/           # 헤더 파일
│  ├─ lib/               # 공용 라이브러리와 자료구조
│  ├─ tests/             # 모듈별 테스트
│  └─ utils/             # pintos 실행 스크립트와 보조 도구
├─ AGENTS.md             # AI 에이전트 작업 기준
├─ CLAUDE.md             # Claude Code 작업 기준
├─ PROJECT1_STUDY_PLAN.md
└─ README.md
```

## 4. 개발 환경

- C
- x86-64 Assembly
- Ubuntu 22.04 DevContainer
- QEMU
- make
- GCC
- GDB
- Python 기반 Pintos 유틸리티

환경 활성화:

```bash
source pintos/activate
```

VM 빌드:

```bash
cd pintos/vm
make
```

VM 테스트:

```bash
cd pintos/vm
make check
```

개별 테스트:

```bash
cd pintos/vm
pintos -- run <test-name>
```

제출 아카이브:

```bash
cd pintos
make TEAM=<team-number> archive
```

## 5. 핵심 파일

- `pintos/vm/vm.c`: VM 초기화, 페이지 할당, 페이지 폴트 처리, SPT 관리
- `pintos/vm/anon.c`: 익명 페이지와 스왑 처리
- `pintos/vm/file.c`: 파일 기반 페이지와 mmap 처리
- `pintos/vm/uninit.c`: 지연 로딩용 uninit page 처리
- `pintos/include/vm/vm.h`: page, frame, SPT 관련 핵심 타입과 API
- `pintos/include/vm/anon.h`: 익명 페이지 인터페이스
- `pintos/include/vm/file.h`: 파일 페이지 인터페이스
- `pintos/userprog/process.c`: 프로세스 로딩, 스택 구성, fork/exec 흐름
- `pintos/userprog/syscall.c`: 시스템 콜 처리와 포인터/파일 검증
- `pintos/threads/thread.c`: 스레드 생명주기와 스케줄링
- `pintos/threads/synch.c`: semaphore, lock, condition 구현

## 6. 커밋 규칙

- 커밋 메시지는 반드시 한국어로 작성합니다.
- 형식은 반드시 Angular Commit Convention을 사용합니다.

```text
type(scope): 변경 내용
```

예시:

```text
feat(threads): alarm clock 구현
fix(synch): priority donation 복원 로직 수정
docs(claude): CLAUDE.md 작업 가이드 문서 추가
chore(git): .gitattributes 추가 및 LF 줄바꿈 규칙 설정
```

## 7. 구현 패턴 규칙

- 기존 모듈의 구현 방식과 스타일을 우선 존중합니다.
- 작은 함수 분리는 좋지만, 운영체제 흐름을 해칠 정도로 과도한 추상화는 피합니다.
- 인터럽트 비활성화 구간은 최소화하고, 필요한 범위에서만 사용합니다.
- 동기화 로직은 테스트 케이스 기대 동작과 함께 검증합니다.
- 스텁 코드를 구현할 때는 테스트 통과뿐 아니라 이후 프로젝트와의 연결성도 고려합니다.
- 디버깅용 출력은 필요 시만 추가하고, 최종 제출 전에는 제거 여부를 점검합니다.
- 빈 구현이나 스텁은 과제 범위상 의도된 상태일 수 있으므로, 테스트와 공식 문서를 함께 확인한 뒤 수정합니다.

## 8. 참고 문서

- KAIST Pintos 공식 문서: https://casys-kaist.github.io/pintos-kaist/
- 프로젝트 README: `README.md`
- 기존 Claude 작업 기준: `CLAUDE.md`
