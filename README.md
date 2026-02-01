# ババ抜き (Old Maid) - WebAssembly Game

CPU対戦型ババ抜きゲーム。C++でゲームロジックを実装し、WebAssemblyにコンパイルしてブラウザ上で動作します。

## プロジェクト概要

| 項目 | 内容 |
|------|------|
| ゲーム名 | ババ抜き (Old Maid) |
| 対戦形式 | 人間 vs CPU |
| 技術スタック | C++ / WebAssembly (Emscripten) / HTML5 / CSS3 / JavaScript |
| 対応ブラウザ | Chrome, Firefox, Safari, Edge (WebAssembly対応ブラウザ) |

### ゲームルール

1. ジョーカー1枚を含む53枚のカードを使用
2. カードを2人に均等に配布
3. ゲーム開始時に同じ数字のペアを自動で捨てる
4. プレイヤーとCPUが交互に相手の手札から1枚引く
5. ペアができたら自動で捨て札へ
6. 最後にジョーカーを持っている方が負け

## フォルダ構成

```
oldmaid/
├── src/                    # C++ ソースコード
│   ├── card.h              # カードクラス定義
│   ├── card.cpp            # カードクラス実装
│   ├── player.h            # プレイヤークラス定義
│   ├── player.cpp          # プレイヤークラス実装
│   ├── game.h              # ゲームクラス定義
│   ├── game.cpp            # ゲームクラス実装
│   └── bindings.cpp        # Emscripten バインディング (C++ ↔ JS)
├── web/                    # フロントエンド
│   ├── index.html          # メインHTML
│   ├── style.css           # スタイルシート
│   └── app.js              # フロントエンドJS
├── build/                  # ビルド出力
│   ├── game.js             # Emscripten生成JS
│   └── game.wasm           # WebAssemblyバイナリ
├── Makefile                # ビルド設定
└── README.md               # このファイル
```

## コードの解説

### C++ クラス構成

#### Card クラス (`src/card.h`, `src/card.cpp`)

カード1枚を表現するクラス。

```cpp
class Card {
    Suit suit;    // スート (HEARTS, DIAMONDS, CLUBS, SPADES, JOKER)
    int rank;     // ランク (1-13: A-K, 0: Joker)

    bool isJoker();              // ジョーカー判定
    bool pairsWith(Card& other); // ペア判定（同じランクか）
    std::string toString();      // 表示用文字列
};
```

#### Player クラス (`src/player.h`, `src/player.cpp`)

プレイヤー（人間またはCPU）を表現するクラス。

```cpp
class Player {
    std::vector<Card> hand;  // 手札
    bool isCPU;              // CPU判定フラグ

    void addCard(Card& card);        // カードを手札に追加
    Card removeCard(int index);      // 指定位置のカードを削除
    std::vector<Card> removePairs(); // 手札からペアを全て削除
    void shuffleHand();              // 手札をシャッフル
};
```

#### Game クラス (`src/game.h`, `src/game.cpp`)

ゲーム全体を管理するクラス。

```cpp
class Game {
    Player players[2];           // プレイヤー配列 [0]:人間, [1]:CPU
    GameState gameState;         // ゲーム状態
    std::vector<Card> discardedPairs;  // 捨て札

    void initGame();                      // ゲーム初期化
    void drawCard(int from, int index);   // カードを引く
    void cpuTurn();                       // CPUのターン実行
    bool checkGameEnd();                  // 終了判定
    int getWinner();                      // 勝者取得
};
```

#### Emscripten バインディング (`src/bindings.cpp`)

C++の関数をJavaScriptから呼び出せるようにするバインディング。

```cpp
EMSCRIPTEN_BINDINGS(oldmaid) {
    function("initGame", &initGame);
    function("getPlayerHand", &getPlayerHand);
    function("getCPUHandSize", &getCPUHandSize);
    function("drawFromCPU", &drawFromCPU);
    function("cpuTurn", &cpuTurn);
    // ...
}
```

### フロントエンド

#### JavaScript API (`web/app.js`)

```javascript
Module.initGame()           // ゲーム開始
Module.getPlayerHand()      // プレイヤーの手札取得 (配列)
Module.getCPUHandSize()     // CPUの手札枚数
Module.drawFromCPU(index)   // CPUから指定位置のカードを引く
Module.cpuTurn()            // CPUのターン実行
Module.getGameState()       // ゲーム状態 (0:未開始, 1:プレイヤーターン, 2:CPUターン, 3:終了)
Module.getDiscardedPairs()  // 捨て札取得
Module.getMessage()         // メッセージ取得
Module.getWinner()          // 勝者 (0:プレイヤー, 1:CPU)
```

## 事前準備と設定・インストール

### 必要なソフトウェア

1. **Emscripten SDK** (WebAssemblyビルド用)
2. **Python 3** (ローカルサーバー用)
3. **モダンブラウザ** (Chrome, Firefox, Safari, Edge)

### Emscripten のインストール

```bash
# リポジトリをクローン
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# 最新版をインストール
./emsdk install latest
./emsdk activate latest

# 環境変数を設定 (Windows)
emsdk_env.bat

# 環境変数を設定 (Linux/Mac)
source ./emsdk_env.sh
```

### ビルド

```bash
# プロジェクトディレクトリで実行
cd oldmaid

# Makefileを使用してビルド
emmake make

# または直接コマンドを実行
em++ -O2 -std=c++17 -s WASM=1 -s MODULARIZE=0 -s ENVIRONMENT='web' \
     -s ALLOW_MEMORY_GROWTH=1 --bind \
     src/card.cpp src/player.cpp src/game.cpp src/bindings.cpp \
     -o build/game.js
```

**Windows (PowerShell) でのビルド例:**

```powershell
C:\emsdk\upstream\emscripten\em++.bat -O2 -std=c++17 -s WASM=1 -s MODULARIZE=0 `
    -s ENVIRONMENT='web' -s ALLOW_MEMORY_GROWTH=1 --bind `
    src/card.cpp src/player.cpp src/game.cpp src/bindings.cpp `
    -o build/game.js
```

## アプリの起動方法

### ローカルサーバーを起動

WebAssemblyはセキュリティ上の理由からローカルファイルとして直接開けないため、HTTPサーバーが必要です。

```bash
# プロジェクトディレクトリで実行
cd oldmaid
python -m http.server 8080
```

### ブラウザでアクセス

```
http://localhost:8080/web/
```

### 操作方法

1. 「ゲーム開始」ボタンをクリック
2. 画面上部のCPUの裏向きカードをクリックして引く
3. CPUが自動であなたの手札から1枚引く
4. 交互に繰り返して、最後にジョーカーを持っている方が負け

## 改善ポイント

### UI/UX

- [ ] カードを引く際のアニメーション追加
- [ ] ペアが捨てられる際のエフェクト
- [ ] サウンドエフェクトの追加
- [ ] モバイル対応の改善（タッチ操作の最適化）
- [ ] カードデザインの改善（画像使用）

### ゲームロジック

- [ ] CPUの思考アルゴリズム強化（ジョーカーを避ける戦略など）
- [ ] 難易度選択機能
- [ ] 複数プレイヤー対応（3人以上）
- [ ] オンライン対戦機能

### 機能追加

- [ ] 戦績の保存（localStorage）
- [ ] リプレイ機能
- [ ] チュートリアルモード
- [ ] 設定画面（アニメーション速度など）

### コード品質

- [ ] 単体テストの追加
- [ ] TypeScript への移行
- [ ] エラーハンドリングの強化
- [ ] コードのドキュメント化

## ネクストアクション

### 短期 (1週間以内)

1. **アニメーション追加**: カードを引く動作とペアを捨てる動作にCSSアニメーションを追加
2. **サウンド追加**: カードを引く音、ペアができた音、勝敗時のサウンド
3. **モバイル最適化**: レスポンシブデザインの調整

### 中期 (1ヶ月以内)

1. **CPUアルゴリズム改善**:
   - ジョーカーの位置を記憶
   - プレイヤーの行動パターン分析
2. **戦績機能**: ローカルストレージに勝敗を保存・表示
3. **設定画面**: ゲーム速度、サウンドON/OFF

### 長期 (3ヶ月以内)

1. **オンライン対戦**: WebSocketを使用したリアルタイム対戦
2. **複数人対戦**: 3〜4人でのババ抜き
3. **ランキングシステム**: オンライン対戦の成績ランキング

## ライセンス

MIT License

## 作者

Created with C++ and WebAssembly
