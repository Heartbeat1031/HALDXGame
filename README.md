# HALDXGame

### インストールと使用方法

1. **Git** がインストールされていない場合は、先にインストールしてください。
   - [Git公式サイト](https://git-scm.com/downloads)
   - [直接ダウンロード（Windows版）](https://github.com/git-for-windows/git/releases/download/v2.49.0.windows.1/Git-2.49.0-64-bit.exe)
   - [インストール手順（日本語）](https://www.sejuku.net/blog/73444#index_id1)
2. **CMake** がインストールされていない場合は、CMakeをインストールしてください。
   - [CMake公式サイト](https://cmake.org/download/)
   - [直接ダウンロード（Windows版）](https://github.com/Kitware/CMake/releases/download/v4.0.2/cmake-4.0.2-windows-x86_64.msi)
3. 上記のインストールが完了したら、任意の場所にフォルダを作成してください。名前はHALDXGameなどがおすすめです。
4. 作成したフォルダ内でコマンドプロンプトを開き、以下のコマンドを実行します：
   ```cmd
   git clone --recurse-submodules https://github.com/ohs40651/HALDXGame.git
   ```
5. Visual Studio で HALDXGame フォルダを開いた後、特に問題がなければしばらく待つと CMake による初期構成が自動で実行されます。

### プロジェクト構成
- assets : アセット
- shaders : シェーダー
- src : 内部ソースコード
  - main.cpp : メイン関数
  - modules : ソースコードモジュール
    - GameFramework : ゲームフレームワーク
    - Physics : 物理エンジン
- third_party : 外部ライブラリ
  - imgui : GUI ライブラリ
  - JoltPhysics : 物理エンジン
  - assimp: 3D モデル読み込みライブラリ

### エラーが出た場合は
1.プロジェクトのルートディレクトリで以下の更新コマンドを実行してください。通常の更新ではサードパーティのプロジェクトが更新されないためです。
```cmd
git submodule update --init --remote
```
2.プロジェクトを削除して再ダウンロードする。

### 余談
1. 世界一使いやすいコードエディタ、**JetBrains**社の **CLion** を強くおすすめします！学生証を使えば、1年間の使用ライセンスを申請できます。
