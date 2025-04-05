#include <windows.h>
#include <tlhelp32.h>
#include <string>

// 提取文件名（不含路径）
std::wstring getFileName(const std::wstring& path) {
    size_t pos = path.find_last_of(L"\\/");
    return (pos != std::wstring::npos) ? path.substr(pos + 1) : path;
}

// 查找进程 PID
DWORD findProcessId(const std::wstring& exeName) {
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(entry);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    DWORD pid = 0;
    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (exeName == entry.szExeFile) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &entry));
    }
    CloseHandle(snapshot);
    return pid;
}

// 终止进程（按进程名）
void killProcessByName(const std::wstring& exeName) {
    DWORD pid = findProcessId(exeName);
    if (pid != 0) {
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
        if (hProcess) {
            TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    //  配置路径（根据你的实际情况修改）
    std::wstring proxyExe = L"D:\\UnblockNeteaseMusic\\unblockneteasemusic-win-x64.exe";
    std::wstring cloudExe = L"D:\\CloudMusic\\cloudmusic.exe";

    //  拼接命令行（仅使用默认参数, 可自行添加)
    std::wstring proxyCmd = L"\"" + proxyExe ;

    //  启动代理（静默）
    STARTUPINFOW si1 = { sizeof(si1) };
    PROCESS_INFORMATION pi1;
    BOOL ok1 = CreateProcessW(NULL, &proxyCmd[0], NULL, NULL, FALSE,
                              CREATE_NO_WINDOW, NULL, NULL, &si1, &pi1);
    if (!ok1) {
        MessageBoxW(NULL, L"启动代理失败，请检查路径或端口是否被占用。", L"错误", MB_ICONERROR);
        return 1;
    }

    //  启动网易云
    STARTUPINFOW si2 = { sizeof(si2) };
    PROCESS_INFORMATION pi2;
    BOOL ok2 = CreateProcessW(cloudExe.c_str(), NULL, NULL, NULL, FALSE,
                              0, NULL, NULL, &si2, &pi2);
    if (!ok2) {
        MessageBoxW(NULL, L"启动网易云客户端失败，请检查路径是否正确。", L"错误", MB_ICONERROR);
        // 若网易云失败，回收代理进程
        TerminateProcess(pi1.hProcess, 0);
        return 2;
    }

    //  等待网易云关闭
    WaitForSingleObject(pi2.hProcess, INFINITE);

    //  网易云退出后关闭代理
    killProcessByName(getFileName(proxyExe));

    return 0;
}
