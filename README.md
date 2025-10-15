# VeyonChat Plugin

VeyonChat is an experimental Veyon plugin that adds a bidirectional chat channel between the Veyon Master and connected clients. It integrates into the Veyon Master toolbar and provides dedicated chat windows on both the master and client sides so teachers can broadcast announcements or have real-time conversations with selected machines.

## Features
- **Master toolbar integration** – `ChatPlugin` registers a `Chat` feature so the master can open the chat window whenever one or more clients are selected.
- **Master chat console** – `MasterChatDialog` lists the currently selected clients, keeps a local message history, supports message priorities (Normal, Urgent, Announcement), and appends timestamps and sender names to each entry.
- **Client chat window** – `ClientChatDialog` shows incoming messages, highlights urgent announcements, and lets students reply; the window floats on top to stay visible during conversations.
- **Keyboard shortcuts** – Both dialogs react to <kbd>F10</kbd> (toggle visibility) and <kbd>Esc</kbd> (minimize/hide) to match the original UX request.
- **Placeholder RPC hooks** – Message dispatch currently logs output and includes TODO blocks that illustrate how to wire Veyon Remote Procedure Calls for real deployments.

## Project structure
| File | Purpose |
| --- | --- |
| `ChatPlugin.h/.cpp` | Entry point that exposes the toolbar feature and spawns the master chat dialog. |
| `MasterChatDialog.h/.cpp` | Qt widget used by the master to chat with selected clients, manage history, and send messages. |
| `ClientChatDialog.h/.cpp` | Qt widget used on the client to display incoming messages and send replies back to the master. |
| `resources.qrc`, `icon.svg` | Qt resource collection and toolbar icon displayed in Veyon Master. |
| `plugin.json` | Plugin metadata consumed by Veyon. |
| `VeyonChat.pro` | Qt project file configured for CI builds against a pre-built Veyon tree. |

## Building
1. Build Veyon from source and note the directory that contains the compiled headers and libraries.
2. Set the `VEYON_BUILD_DIR` environment variable to that directory before invoking `qmake`/Qt Creator. The `.pro` file uses it to locate `VeyonCore` and `VeyonMaster` headers and libraries.
3. Configure a **Release** build in Qt Creator (or via the CLI) and build the project. The output is a shared library (`VeyonChat.dll`/`libVeyonChat.so`).
4. Copy the resulting library and `plugin.json` into a `VeyonChat` subfolder inside Veyon’s `plugins` directory on the target machines.

## Next steps
- Replace the pseudo-code in the send/receive methods with real Veyon RPC calls so messages travel over the established Veyon connection.
- Persist chat history or export transcripts if needed.
- Add polish such as presence status, typing indicators, or localization.

With those additions the plugin will be ready for classroom pilots.
