#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

static int ShowError(const wchar_t* message, int code) {
    MessageBoxW(nullptr, message, L"Qwen3-TTS Spark", MB_OK | MB_ICONERROR);
    return code;
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR arguments, int) {
    std::vector<wchar_t> modulePath(32768);
    const DWORD length = GetModuleFileNameW(nullptr, modulePath.data(),
                                            static_cast<DWORD>(modulePath.size()));
    if (length == 0 || length >= modulePath.size()) {
        return ShowError(L"Could not locate the launcher.\n无法找到启动器位置。", 1);
    }

    fs::path root = fs::path(modulePath.data()).parent_path();
    std::error_code error;
    if (!fs::is_regular_file(root / L"app.py", error)) {
        root /= L"Qwen3-TTS-Spark";
    }
    const fs::path python = root / L"runtime" / L"python.exe";
    const fs::path app = root / L"app.py";
    const fs::path torch = root / L"runtime" / L"Lib" / L"site-packages" / L"torch" / L"lib" / L"torch_cuda.dll";
    const fs::path model = root / L"models" / L"Qwen3-TTS-12Hz-0.6B-CustomVoice" / L"model.safetensors";
    if (!fs::is_regular_file(python, error) || !fs::is_regular_file(app, error) ||
        !fs::is_regular_file(torch, error) || !fs::is_regular_file(model, error)) {
        return ShowError(
            L"Extract all four release ZIPs into the same parent folder, then put this EXE next to the Qwen3-TTS-Spark folder or inside it.\n\n"
            L"请把发行版的四个 ZIP 解压到同一目录，再将此 EXE 放到 Qwen3-TTS-Spark 文件夹旁边或里面。", 2);
    }

    std::wstring command = L"\"" + python.wstring() + L"\" -I \"" + app.wstring() + L"\"";
    if (arguments != nullptr && *arguments != L'\0') {
        command += L" ";
        command += arguments;
    }

    STARTUPINFOW startup{};
    startup.cb = sizeof(startup);
    PROCESS_INFORMATION process{};
    if (!CreateProcessW(python.c_str(), command.data(), nullptr, nullptr, FALSE,
                        CREATE_NEW_CONSOLE, nullptr, root.c_str(), &startup, &process)) {
        return ShowError(L"Could not start the bundled Python runtime.\n无法启动随包携带的 Python。", 3);
    }
    CloseHandle(process.hThread);
    const DWORD wait = WaitForSingleObject(process.hProcess, INFINITE);
    DWORD status = 1;
    const BOOL queried = GetExitCodeProcess(process.hProcess, &status);
    CloseHandle(process.hProcess);
    if (wait != WAIT_OBJECT_0 || !queried || status != 0) {
        return ShowError(L"Qwen3-TTS exited with an error. Check the extracted files and NVIDIA driver.\n请检查解压文件和 NVIDIA 驱动。", 4);
    }
    return 0;
}
