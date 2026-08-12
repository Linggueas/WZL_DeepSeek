# DeepSeek AI 智能对话客户端

一款基于 Qt 6 和 DeepSeek API 构建的桌面端 AI 对话工具，支持多会话管理、流式输出、深度思考模式、文件上传与本地持久化存储。
<img width="2004" height="1460" alt="image" src="https://github.com/user-attachments/assets/ab1040bf-ba3e-4721-85ca-67a9f551b23a" />

## 功能特性

- **多会话管理** — 支持创建多个独立对话会话，会话之间数据完全隔离，互不干扰
- **流式实时输出** — AI 回复以 token 流式逐字展示，配合批处理定时器刷新，响应即时且界面流畅
- **深度思考模式** — 支持 DeepSeek 的 reasoning_content 思考过程展示，可实时查看模型推理链路
- **Markdown 渲染** — 使用自定义 QStyledItemDelegate 渲染 AI 回复中的 Markdown 内容，支持 Ctrl+滚轮缩放字号
- **问题列表导航** — 左侧问题列表展示当前会话的所有提问，点击即可跳转到对应问题；支持一键隐藏/显示
- **查看原文** — 右键点击任意消息，可在独立窗口中查看未经 Markdown 修饰的纯文本原文并自由复制
- **文件上传** — 支持上传本地文本文件，自动嵌入到对话上下文中发送给模型
- **对话历史持久化** — 基于 SQLite 本地存储所有对话记录，支持回溯和管理
- **错误处理** — 网络请求失败时弹出错误详情（含服务端返回信息）并立即停止当前对话
- **导出功能** — 对话回答可导出为 txt 文件
- **多模型切换** — 支持通过下拉框切换不同的 DeepSeek 模型

## 系统架构

该项目遵循 **Qt MVC (Model-View-Controller)** 设计模式：

- **View（视图）** — 维护一个统一的 QListView 作为对话展示视图
- **Model（模型）** — 每个对话会话独立分配一个 QStringListModel，确保界面高性能响应
- **数据隔离** — 每个会话使用独立的 QJsonArray 存储历史消息，避免上下文混淆

### 核心模块

| 模块 | 文件 | 职责 |
|------|------|------|
| **主窗口** | `mainwindow.{h,cpp}` | 整体 UI 布局、事件处理、会话管理与问题列表导航 |
| **HTTP 管理器** | `httpmgr.{h,cpp}` | 封装 QNetworkAccessManager，SSE 流式解析，错误信号上报 |
| **SQL 管理器** | `sqlmgr.{h,cpp}` | 封装 QSqlDatabase，负责 SQLite 的读写操作 |
| **Markdown 委托** | `markdowndelegate.{h,cpp}` | 自定义 QStyledItemDelegate，渲染 Markdown 内容 |
| **文件列表对话框** | `filelistdialog.{h,cpp}` | 管理已上传文件的列表展示与删除 |
| **文件查看对话框** | `fileviewdialog.{h,cpp}` | 预览上传的文件内容 |
| **文件组件** | `filedialog.{h,cpp}` | 单个文件条目的 UI 展示 |
| **删除确认对话框** | `deletedialog.{h,cpp}` | 删除会话时的二次确认弹窗 |
| **自定义 ListView** | `my_list_view.{h,cpp}` | 支持右键删除等扩展操作的 QListView |

### 数据流

```
用户输入 → MainWindow 构建 JSON → HttpMgr POST 请求 → DeepSeek API
                                                              ↓
MainWindow 更新 UI ← slot_read_data 逐 token 接收 ←  SSE 流式响应
                                                              ↓
        错误 → QMessageBox 提示并停止对话 ← requestError 信号
                                                              ↓
SQLMgr 保存对话历史 ← streamFinished 结束信号 ←  收到 [DONE]
```

## 环境要求

- **Qt 版本** — Qt 6.x（项目使用 Qt 6.9.1）
- **编译器** — 支持 C++17 的编译器（如 MinGW 64-bit、MSVC）
- **模块依赖**
  - `qt.network` — 网络请求
  - `qt.sql` — SQLite 数据库

## 构建与运行

### 使用 Qt Creator

1. 打开 `deepseek_client.pro` 项目文件
2. 选择 Qt 6.x Kit（如 `Desktop Qt 6.9.1 MinGW 64-bit`）
3. 点击构建并运行

### 使用命令行

```bash
# 生成 Makefile
qmake deepseek_client.pro

# 构建（Windows/MinGW）
mingw32-make

# 构建（Linux/macOS）
make

# 运行
./deepseek_client
```

## 配置 API Key

程序启动时从程序目录下的 `api.txt` 读取 API Key 并填入顶部输入框，也可直接在输入框中修改（修改会即时生效并写回 `api.txt`）。

> **注意**：请勿将 `api.txt` 提交到版本库，以免泄露密钥。

## 使用说明

1. **新建会话** — 点击"新建对话"按钮创建新会话
2. **发送消息** — 在输入框输入内容，点击发送按钮
3. **深度思考** — 点击"深度思考"按钮切换 reasoning 模式，可实时查看模型推理过程
4. **上传文件** — 点击右下角文件按钮选择本地文本文件上传
5. **切换会话** — 点击左侧列表切换不同会话
6. **问题列表导航** — 点击右下角"?"按钮展开问题列表，点击问题即可跳转到对话中的对应位置
7. **查看原文** — 右键点击对话中的任意消息，弹出纯文本原文窗口，可 Ctrl+C 复制
8. **删除会话** — 右键点击会话列表中的条目进行删除
9. **切换模型** — 通过顶部下拉框切换不同 DeepSeek 模型

## 许可证

本项目基于 [MIT License](LICENSE) 开源。

版权所有 (c) 2026 Linggueas
