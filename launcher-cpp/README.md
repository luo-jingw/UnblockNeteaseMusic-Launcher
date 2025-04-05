# NeteaseMusicLauncher (Windows C++)

A lightweight Windows launcher that:

- Silently starts UnblockNeteaseMusic (on port 8080)
- Launches the Netease Cloud Music client
- Monitors the client process and shuts down the proxy when it exits
- Shows no command window, ideal for taskbar pinning

## 🚀 How to Use

1. Download `unblockneteasemusic-win-x64.exe` from:
   https://github.com/UnblockNeteaseMusic/server/releases

2. Install the official [Netease Cloud Music](https://music.163.com/) client

3. Modify the paths in `NeteaseMusic.cpp` if needed:

```cpp
std::wstring proxyExe = L"D:\\UnblockNeteaseMusic\\unblockneteasemusic-win-x64.exe";
std::wstring cloudExe = L"D:\\CloudMusic\\cloudmusic.exe";
```

4. Compile:

```bash
g++ NeteaseMusic.cpp -o NeteaseMusic.exe -mwindows
```

5. Create a shortcut, assign `NeteaseMusic.ico`, and pin it to your taskbar.

## 📝 License

MIT