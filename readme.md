# DeepSeek AI 智能对话客户端

一款基于 Qt 6 和 DeepSeek API 构建的桌面端 AI 对话工具，支持多会话管理、流式输出、深度思考模式、文件上传与本地持久化存储。

## 功能特性

- **多会话管理** — 支持创建多个独立对话会话，会话之间数据完全隔离，互不干扰
- **流式实时输出** — AI 回复以 token 流式逐字展示，响应即时可见
- **深度思考模式** — 支持 DeepSeek 的 reasoning_content 思考过程展示，可实时查看模型推理链路
- **Markdown 渲染** — 使用自定义 QStyledItemDelegate 渲染 AI 回复中的 Markdown 内容
- **文件上传** — 支持上传本地文本文件，自动嵌入到对话上下文中发送给模型
- **对话历史持久化** — 基于 SQLite 本地存储所有对话记录，支持回溯和管理
- **导出功能** — 对话回答可导出为 txt 文件
- **多模型切换** — 支持通过下拉框切换不同的 DeepSeek 模型（如 v4-flash 等）

## 系统架构

该项目遵循 **Qt MVC (Model-View-Controller)** 设计模式：

- **View（视图）** — 维护一个统一的 QListView 作为对话展示视图
- **Model（模型）** — 每个对话会话独立分配一个 QStringListModel，确保界面高性能响应
- **数据隔离** — 每个会话使用独立的 QJsonArray 存储历史消息，避免上下文混淆

### 核心模块

| 模块 | 文件 | 职责 |
|------|------|------|
| **主窗口** | `mainwindow.{h,cpp}` | 整体 UI 布局、事件处理、会话管理 |
| **HTTP 管理器** | `httpmgr.{h,cpp}` | 封装 QNetworkAccessManager，与 DeepSeek API 通信 |
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
SQLMgr 异步保存 ← streamFinished 结束信号 ←  响应完成
```

## 环境要求

- **Qt 版本** — Qt 6.x（项目使用 Qt 6.9.1）
- **编译器** — 支持 C++17 的编译器（如 MinGW 64-bit、MSVC）
- **模块依赖**
  - `qt.network` — 网络请求
  - `qt.sql` — SQLite 数据库
  - `qt.concurrent` — 异步数据库操作

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

> **注意**: 项目中的 API Key 硬编码在 `httpmgr.cpp` 中（`sk-36d1b631abfd0fd`），建议在部署前更换为自己的 DeepSeek API Key。

## 使用说明

1. **新建会话** — 点击"新建对话"按钮创建新会话
2. **发送消息** — 在输入框输入内容，点击发送按钮
3. **深度思考** — 点击"深度思考"按钮切换 reasoning 模式，可实时查看模型推理过程
4. **上传文件** — 点击文件按钮（右下角）选择本地文本文件上传
5. **切换会话** — 点击左侧列表切换不同会话
6. **删除会话** — 右键点击会话列表中的条目进行删除
7. **切换模型** — 通过顶部下拉框切换不同 DeepSeek 模型

## 许可证

本项目基于 [MIT License](LICENSE) 开源。

版权所有 (c) 2026 Linggueas

