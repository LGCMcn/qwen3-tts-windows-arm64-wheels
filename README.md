# Qwen3-TTS audio wheels for Windows ARM64

Community builds for native Windows on ARM. Tested on an NVIDIA RTX Spark N1X with ARM64 CPython 3.14.10 and an existing `torch 2.15.0+cu135` installation. Compatibility with other PyTorch versions, Python versions, and devices has not been verified.

## Install

### Install with Git / 用 Git 获取并安装

激活目标虚拟环境后，在 PowerShell 中运行这一行。Git 克隆成功后，pip 会从 Release 安装两个 wheel。

```powershell
git clone https://github.com/LGCMcn/qwen3-tts-windows-arm64-wheels.git; if ($LASTEXITCODE -eq 0) { python -m pip install --no-deps -r .\qwen3-tts-windows-arm64-wheels\requirements-wheels.txt }
```


### One-command install / 一键安装

在目标虚拟环境中运行下面这条 PowerShell 命令。它从 GitHub Release 直接下载两个 wheel，并保留已有的 PyTorch。

```powershell
python -m pip install --no-deps "https://github.com/LGCMcn/qwen3-tts-windows-arm64-wheels/releases/download/v2026.11.17/soxr-1.1.0-cp312-abi3-win_arm64.whl" "https://github.com/LGCMcn/qwen3-tts-windows-arm64-wheels/releases/download/v2026.11.17/torchaudio-2.11.0a0%2B9b19155-py3-none-win_arm64.whl"
```

### Manual download


Download both wheels from [Releases](https://github.com/LGCMcn/qwen3-tts-windows-arm64-wheels/releases). Open PowerShell in the download directory with your target virtual environment activated. Check that the interpreter reports ARM64 and that your existing PyTorch imports before installing.

```powershell
python -c "import platform, sys, torch; print(sys.executable, platform.machine(), sys.version, torch.__version__, torch.cuda.is_available())"
python -m pip install --no-deps .\soxr-1.1.0-cp312-abi3-win_arm64.whl .\torchaudio-2.11.0a0+9b19155-py3-none-win_arm64.whl
python -m pip check
```

The wheel installation command preserves your current PyTorch and assumes the environment already has the other Qwen3-TTS dependencies. On a fresh environment, install the remaining dependencies using the requirements from your Qwen3-TTS checkout. A constraint matching your exact installed torch version can prevent the resolver from substituting a different build.

## Build provenance and licenses

- **python-soxr 1.1.0** comes from the unmodified PyPI source distribution attached to the release as `soxr-1.1.0.tar.gz`. The wheel bundles statically linked libsoxr. The source archive includes the corresponding code and build files, so you can rebuild with your own modified libsoxr. License notices are included in the wheel under `soxr-1.1.0.dist-info/licenses/`. [Upstream source and PyPI distribution](https://pypi.org/project/soxr/1.1.0/#files)
- **TorchAudio 2.11.0a0+9b19155** was built from upstream commit `9b19155e6fce04a83704eadc85d3dfe99e520c0c`. The Windows ARM64 native extension was built with CUDA, ROCm and OpenMP disabled. The wheel includes the upstream BSD license. Qwen3-TTS can still perform inference using the GPU through PyTorch. [Exact upstream commit](https://github.com/pytorch/audio/tree/9b19155e6fce04a83704eadc85d3dfe99e520c0c)

To rebuild, activate the existing ARM64 Python environment in a Visual Studio 2022 Developer Command Prompt targeting ARM64. Install the ARM64 C++ build tools and Windows SDK. Use the following commands in that Command Prompt with the source archive in the current directory.

```bat
python -m pip install ninja wheel
set CMAKE_GENERATOR=Ninja
python -m pip wheel --no-deps -w dist soxr-1.1.0.tar.gz

git -c core.longpaths=true clone https://github.com/pytorch/audio.git
git -C audio config core.longpaths true
git -C audio checkout 9b19155e6fce04a83704eadc85d3dfe99e520c0c

set VSLANG=1033
chcp 65001
set PYTHONUTF8=1
set DISTUTILS_USE_SDK=1
set USE_CUDA=0
set USE_ROCM=0
set USE_OPENMP=0
set TORCH_DONT_CHECK_COMPILER_ABI=1
python -m pip wheel --no-build-isolation --no-deps -w dist audio
```

The ABI-check override works around decoding of localized MSVC output on the build host. Use the compiler and PyTorch combination documented above. To relink with a modified libsoxr, extract the attached source archive, replace or modify its `libsoxr` subtree, and pass the extracted directory to the same `pip wheel` command.

## Validation scope

The original build passed `pip check`, Kaldi filter-bank extraction, resampling, and GPU synthesis with Qwen3-TTS 0.1.1 and the official 12Hz 0.6B CustomVoice model. The release wheels were checked for native import, filter-bank extraction, resampling and CUDA tensor execution. The SoX executable is separate. The 25Hz path was not tested.

## 中文说明

这两个 wheel 面向 Windows ARM64。已验证的组合是原生 ARM64 Python 3.14.10、`torch 2.15.0+cu135` 和 RTX Spark N1X。下载 Release 中的两个 wheel，在自己的虚拟环境里用上面的 `--no-deps` 命令安装，可以保留现有 PyTorch。SHA-256 校验值见 Release 附件。


