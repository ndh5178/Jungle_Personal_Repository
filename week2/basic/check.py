#!/usr/bin/env python3
"""
Algorithm practice checker script.

Usage:
  python check.py <problem_file>
  python check.py --all
"""

import os
import subprocess
import sys


def decode_output(data: bytes) -> str:
    """Decode subprocess output robustly across UTF-8/CP949 environments."""
    try:
        return data.decode("utf-8").strip()
    except UnicodeDecodeError:
        return data.decode("cp949", errors="replace").strip()


def check_solution(problem_file):
    """Run a problem file and compare stdout with the expected output file."""
    if not os.path.exists(problem_file):
        return False, f"파일을 찾을 수 없습니다: {problem_file}"

    base_name = problem_file.replace(".py", "")
    output_file = f"{base_name}_output.txt"

    if not os.path.exists(output_file):
        return False, f"정답 파일을 찾을 수 없습니다: {output_file}"

    try:
        result = subprocess.run(
            [sys.executable, problem_file],
            capture_output=True,
            text=False,
            timeout=10,
        )

        stderr_text = decode_output(result.stderr)
        if result.returncode != 0:
            return False, f"실행 오류:\n{stderr_text}"

        actual_output = decode_output(result.stdout)

        with open(output_file, "r", encoding="utf-8") as f:
            expected_output = f.read().strip()

        actual_lines = [line.strip() for line in actual_output.split("\n") if line.strip()]
        expected_lines = [line.strip() for line in expected_output.split("\n") if line.strip()]

        if actual_lines == expected_lines:
            return True, "PASS - 정답입니다"

        diff_msg = "FAIL - 출력이 다릅니다.\n\n"
        diff_msg += "=== 예상 출력 ===\n"
        diff_msg += expected_output[:500]
        if len(expected_output) > 500:
            diff_msg += "\n... (생략)"

        diff_msg += "\n\n=== 실제 출력 ===\n"
        diff_msg += actual_output[:500]
        if len(actual_output) > 500:
            diff_msg += "\n... (생략)"

        return False, diff_msg

    except subprocess.TimeoutExpired:
        return False, "시간 초과 (10초)"
    except Exception as e:
        return False, f"오류 발생: {str(e)}"


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return

    if sys.argv[1] == "--all":
        print("=" * 60)
        print("Week2 전체 문제 채점")
        print("=" * 60)
        print()

        problem_files = [
            "01_python_dict.py",
            "02_array.py",
            "03_string.py",
            "04_brute_force.py",
            "05_recursion.py",
            "06_backtracking.py",
            "07_complexity.py",
            "08_bubble_sort.py",
            "09_insertion_sort.py",
            "10_number_theory.py",
        ]

        passed = 0
        failed = 0

        for problem_file in problem_files:
            print(f"- {problem_file}")
            success, message = check_solution(problem_file)
            print(f"  {message}")

            if success:
                passed += 1
            else:
                failed += 1
            print()

        print("=" * 60)
        print(f"결과: {passed}개 통과, {failed}개 실패 (총 {passed + failed}개)")
        print("=" * 60)
        return

    problem_file = sys.argv[1]
    print("=" * 60)
    print(f"문제: {problem_file}")
    print("=" * 60)
    print()

    success, message = check_solution(problem_file)
    print(message)
    print()

    if success:
        print("축하합니다! 문제를 해결했습니다.")
    else:
        print("힌트: TODO 부분을 다시 확인해보세요.")


if __name__ == "__main__":
    main()
