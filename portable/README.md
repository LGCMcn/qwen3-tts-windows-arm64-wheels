# Qwen3-TTS Spark Portable 2026.11.17

面向 **Windows on Arm + NVIDIA RTX Spark** 的社区配音包。双击启动器后，在本地网页输入文本、选择音色并下载 WAV。

## 解压和运行

[下载全部 ZIP 分卷](https://github.com/LGCMcn/qwen3-tts-windows-arm64-wheels/releases/tag/portable-v2026.11.17)

1. 下载 Release 中的全部 ZIP 附件。
2. 把全部 ZIP 解压到同一个父目录，合并包内的 `Qwen3-TTS-Spark` 文件夹。压缩包按内容拆分，无需拼接文件。
3. 下载 `Qwen3-TTS-Spark.exe`，放到解压出的 `Qwen3-TTS-Spark` 文件夹旁边或里面，然后双击 EXE。也可双击包内的 `Start-Qwen3-TTS.cmd`。应用、Python、CUDA 版 PyTorch 和模型都在包内，启动及配音无需联网、pip 或管理员权限。目标设备需已安装兼容的 NVIDIA 驱动。


也可以在解压后的文件夹里用 PowerShell 生成 WAV：

```powershell
.\Start-Qwen3-TTS.cmd --text "你好，这是一段配音。" --speaker Vivian --language Chinese --output output.wav
```

## 包含的模型和验证的版本

| 项目 | 验证值 |
|---|---|
| 设备 | ASUS ProArt P16 H7607BA |
| GPU | NVIDIA RTX Spark N1X，6144 核 Blackwell RTX GPU |
| 系统 | Windows 11 家庭中文版 26H1，ARM64，10.0.28000.3501 |
| NVIDIA 驱动 | 617.69 |
| CUDA | PyTorch 构建为 CUDA 13.5（cu135），本机 NVIDIA-SMI 报告 CUDA UMD 13.4 |
| Python | CPython 3.14.10，原生 Windows ARM64 |
| PyTorch | NVIDIA 官方 2.15.0+cu135，提交 31a77fc9c0a7c6fff15a1ff58c743f53b1a9d457 |
| Qwen3-TTS | qwen-tts 0.1.1，源码提交 022e286b98fbec7e1e916cb940cdf532cd9f488e |
| 模型 | Qwen3-TTS-12Hz-0.6B-CustomVoice，含 12Hz speech tokenizer |
| 推理 | CUDA BF16，PyTorch SDPA，24 kHz WAV |
| 音频包 | soxr 1.1.0，TorchAudio 2.11.0a0+9b19155 |

此包提供 CustomVoice 预设音色。25Hz、声音克隆、VoiceDesign 和其他设备组合未验证。首启可能需要几分钟加载 Python 库。

## 来源与许可

- [Qwen3-TTS 代码](https://github.com/QwenLM/Qwen3-TTS/tree/022e286b98fbec7e1e916cb940cdf532cd9f488e)与[官方模型](https://huggingface.co/Qwen/Qwen3-TTS-12Hz-0.6B-CustomVoice)，Apache 2.0。模型主权重 SHA-256 为 bc3c7e785eb961179c25450d1acff03f839e0002f2f3a5aeb67b5735c0fa2adb。
- [NVIDIA PyTorch 官方索引](https://pypi.nvidia.com/nvtorch_oot/torch/)中的 wheel 是包内 PyTorch 运行时的来源。原始 wheel 的 SHA-256 为 8413e33e10fac2692c1b9c22c7544c5af4eb3b225376e2e27699820519c5a3b2。适用条款见官方分发及其附带的许可证。
- [soxr 和 TorchAudio 的源码与构建来源](https://github.com/LGCMcn/qwen3-tts-windows-arm64-wheels#build-provenance-and-licenses)。包内附 python-soxr 对应源码及第三方组件的许可证。


使用随包提供的商业运行时前请查看 licenses/RUNTIME-NOTICES.md 与附带条款。


发布包共约 4.66 GB，包含四个 ZIP。随附 SHA256SUMS.txt。
已在含中文和空格的新目录中解压四个 ZIP，使用空模型缓存和不可用的外部代理验证命令行及本地网页均可生成 24 kHz WAV。

