# P2996 Reflection Demo

Experimental C++26 static reflection implementation demonstrating member lookup, type information, serialization, and property modification using [P2996](https://wg21.link/p2996).

**Research Context:** This repository contains proof-of-concept code supporting academic research comparing P2996's compile-time reflection with Unreal Engine's runtime reflection system. For detailed analysyis, findings, and comparitive evaluation, see the full research paper. For detailed analysis, findings, ands on initial reflection comparisons. The comparative evaluation, see the full research paper.

This project expands on initial reflection comparisons. The baseline project focuses on fundamental reflection capabilities: member lookup, type information, JSON serialization, and dynamic property modification.

## Quick Run

### Pre-built compiler included

```bash
git clone https://github.com/liamcremers/ReflectionProject.git
cd p2996-ssh
./setup.sh
./build/Reflection
./watch.sh
```

## Why Linux Only?

This demo requires **libc++** (LLVM's C++ standard library) built with the P2996 compiler for reflection features to work. While theoretically possible on Windows, libc++ on Windows with clang-cl has significant build challenges and is not officially supported by default in LLVM builds. The pre-built toolchain in this repo was compiled on Linux and heavily inspired by this [dockerfile](https://github.com/simdjson/experimental_json_builder/blob/main/Dockerfile).

## What's Included

- `p2996-install/`  Pre-built P2996 clang compiler with libc++ (7.3GB, saves hours of compilation)
- `main.cpp` Reflection examples showcasing P2996 capabilities highlighting:
  - Member lookup across struct hierarchies
  - Runtime type information access
  - Automatic JSON serialization/deserialization
  - Dynamic property modification without macros
- `setup.sh` Installs required dependencies and builds project
- `watch.sh` Monitors input.json and runs project when change detected

## Research Background

This project explores **C++ static reflection** as proposed in [P2996](https://wg21.link/p2996) and compares it with Unreal Engine's Header Tool (UHT) reflection system.

### Key Research Hypotheses

**H1:** P2996 offers **cleaner** syntax with reduced macro overhead compared to Unreal's reflection system

**H2:** P2996 **cannot** replicate Unreal Engine's dynamic runtime capabilities

**H3:** P2996 offers **better** compile-time type safety and error detection compared to UE's macro-based approach

## Research Focus: Member Lookup, Type Information, Serialization & Property Modification

This project evaluates how P2996 and Unreal Engine handle four core reflection capabilities:

### Serialization & Deserialization Workflow

**P2996 Approach:**

- Compile-time member enumeration via `nonstatic_data_members_of()`
- Type-safe access with splicing operators (`:[type_of(member):]`)
- Recursive reflection for nested aggregates
- No macro annotations required
- Using nlohmann to handle json

**Unreal Engine Approach:**

- Runtime UPROPERTY macros for member marking
- Built-in `FJsonObjectConverter` for string-to-struct conversion
- Requires explicit markup on each member
- Automatic handling via reflection metadata

### Comparative Analysis

| Feature | P2996 | Unreal Engine | Notes |
|---------|-------|---------------|-------|
| **Member Lookup** | ✓ | ✓ | Both enumerate struct members |
| **Type Information** | ✓ | ✓ | Both access member types at compile/runtime |
| **Serialization** | ✓ | ✓ | P2996 generic, UE requires converter |
| **Property Modification** | ✓ | ✓ | Both support dynamic value assignment |
| **Runtime Type Creation** | ✗* | ✗ | Neither creates structs from unknown schemas |
| **New Member Introduction** | ✗* | ✗ | Both locked at compile-time |

---

<!-- Results section with data -->
<details>
<summary><b>WSL Setup (Windows Users)</b></summary>

### Install WSL & Clone

```bash
# In PowerShell
wsl --install
wsl

# Inside WSL
sudo apt update
git clone https://github.com/liamcremers/ReflectionProject.git ~/p2996-ssh
cd ~/p2996-ssh
./setup.sh
code --remote wsl+Ubuntu .
./watch.sh
```

### One-liner for Existing WSL Setup

```bash
wsl bash -c "git clone https://github.com/liamcremers/ReflectionProject.git ~/p2996-ssh && cd ~/p2996-ssh && bash setup.sh && bash watch.sh"
```

### Install [Github CLI tool](https://raw.githubusercontent.com/cli/cli/refs/heads/trunk/docs/install_linux.md#:~:text=To,%60,-To)

```bash
(type -p wget >/dev/null || (sudo apt update && sudo apt install wget -y)) \
 && sudo mkdir -p -m 755 /etc/apt/keyrings \
 && out=$(mktemp) && wget -nv -O$out https://cli.github.com/packages/githubcli-archive-keyring.gpg \
 && cat $out | sudo tee /etc/apt/keyrings/githubcli-archive-keyring.gpg > /dev/null \
 && sudo chmod go+r /etc/apt/keyrings/githubcli-archive-keyring.gpg \
 && sudo mkdir -p -m 755 /etc/apt/sources.list.d \
 && echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | sudo tee /etc/apt/sources.list.d/github-cli.list > /dev/null \
 && sudo apt update \
 && sudo apt install gh -y

  gh auth login
```

</details>

<details>
<summary><b>Building P2996 Compiler from Source</b></summary>
If you want to build the compiler yourself instead of using the pre-built version:

```bash
sudo apt-get update && sudo apt-get install -y \
  build-essential cmake ninja-build git python3 zlib1g-dev

git clone --depth=1 --branch p2996 https://github.com/bloomberg/clang-p2996.git p2996

cmake -S p2996/llvm -B p2996/build-llvm -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_ASSERTIONS=ON \
  -DLLVM_UNREACHABLE_OPTIMIZE=ON \
  -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind" \
  -DCLANG_DEFAULT_CXX_STDLIB=libc++ \
  -DLLVM_ENABLE_PROJECTS=clang

cmake --build p2996/build-llvm -j $(nproc)  # Takes 2+ hours
cmake --install p2996/build-llvm --prefix $PWD/p2996-install
```

</details>

<details>
<summary><b>Building P2996 Compiler with Docker</b></summary>

Using Docker provides an isolated build environment. This approach is based on the [simdjson experimental_json_builder](https://github.com/simdjson/experimental_json_builder/blob/main/Dockerfile) Dockerfile.
</details>
<details>
<summary><b>VS Code Setup (clangd)</b></summary>

**Important:** Disable C/C++ IntelliSense and use the P2996-built clangd for proper reflection support.

Add to `.vscode/settings.json`:

```json
{
  "clangd.path": "${workspaceFolder}/clangd",
  "clangd.arguments": [],
  "C_Cpp.intelliSenseEngine": "disabled",
}
```

This ensures VS Code uses the P2996-aware clangd that understands reflection syntax.

</details>

## References

- [P2996R13 - Reflection for C++26](https://wg21.link/p2996)
- [Bloomberg Clang/P2996 Implementation](https://github.com/bloomberg/clang-p2996)
- [Experimental_json_builder/Dockerfile](https://github.com/simdjson/experimental_json_builder/blob/main/Dockerfile)
- more credits in the paper...

---

**Note:** This is research code exploring C++26 reflection proposals. Production use should await C++26 compiler availability and standard finalization.
