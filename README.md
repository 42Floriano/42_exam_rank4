# Rank 4 (new) - 42 Exam Repository

This repository contains solutions and resources for the Rank 4 exam at 42. It is structured into three main folders, each serving a distinct purpose.

## 📁 Repository Structure

```
├── pipe_lessons/      # Contains lesson files about pipes (Not essential)
├── level_01/          # Level 1 exercises
│   ├── ft_popen.c     # Exercise 1 - ft_popen
│   ├── picoshell.c    # Exercise 2 - picoshell
│   ├── sandbox.c      # Exercise 3 - sandbox
├── level_02/          # Level 2 exercises
│   ├── argo/          # Exercise 1 - argo
│   │   ├── given/     # Provided files and subject (additional test cases needed)
│   │   ├── solution/  # Solution files and testers
│   ├── vbc/           # Exercise 2 - vbc
│   │   ├── given/     # Provided files and subject
│   │   ├── solution/  # Solution files and testers
│   ├── tsp/           # Exercise 3 - tsp (Obsolete)
```

---

## 📌 Level 1 Exercises
Each exercise in `level_01` is a `.c` file containing its subject as comments within the file and a testable `main()` function.
Each exercise in `level_01` is a `.c` file containing its subject and a testable `main()` function.

### 🚀 Compiling and Running Level 1 Exercises
Navigate to the desired file's location and compile it using `gcc` with the required flags:

Example:
```sh
cd level_01
gcc -Werror -Wextra -Wall ft_popen.c -o ft_popen
./ft_popen
```

Repeat the same process for `picoshell.c` and `sandbox.c`.

---

## 📌 Level 2 Exercises
Each exercise in `level_02` consists of two subfolders:
- **given/**: Contains the original files provided at the start of the exam and the subject (similar to the real exam but requires additional test cases).
- **solution/**: Contains the solution files along with necessary testers.

### 🔧 Running Testers
#### ✅ VBC Tester
The `solution/` folder for `vbc` contains two shell script testers:
- **test_linux.sh** (for Linux)
- **test_mac.sh** (for macOS)

Before running, make the script executable:
```sh
chmod 777 test_linux.sh
./test_linux.sh
```

#### ✅ Argo Tester
The tester for `argo` is inside `argo.c`. To test it:
1. Uncomment the `main()` function in `argo.c`.
2. Compile the file with the required flags:
   ```sh
   gcc -Werror -Wextra -Wall argo.c argo.h -o argo_tester
   ./argo_tester
   ```

---

## ⚠️ Notes
- The `pipe_lessons/` folder is not crucial for the exam but may provide useful insights.
- The `tsp/` exercise in `level_02` is **obsolete** and should not be used.
- Ensure you have the correct permissions before executing any shell scripts (`chmod 777`).

---

### 🏆 Good luck with your Rank 4 exam! 🚀

