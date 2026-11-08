// ババ抜き UI ロジック
// ゲーム本体は Emscripten (build/game.js) が Module に公開する関数を使う:
//   initGame(), getPlayerHand(), getCPUHandSize(), drawFromCPU(index),
//   cpuTurn(), getGameState(), getDiscardedPairs(), getMessage(),
//   getCurrentTurn(), getWinner(), getPlayerHandSize()
// GameState: 0=NOT_STARTED, 1=PLAYER_TURN, 2=CPU_TURN, 3=GAME_OVER

const STATE = { NOT_STARTED: 0, PLAYER_TURN: 1, CPU_TURN: 2, GAME_OVER: 3 };
const SUITS = [
    { symbol: '♠', name: 'spades' },
    { symbol: '♥', name: 'hearts' },
    { symbol: '♦', name: 'diamonds' },
    { symbol: '♣', name: 'clubs' },
];

let cpuTimer = null;

function whenModuleReady(callback) {
    if (typeof Module !== 'undefined' && typeof Module.initGame === 'function') {
        callback();
        return;
    }
    const timer = setInterval(() => {
        if (typeof Module !== 'undefined' && typeof Module.initGame === 'function') {
            clearInterval(timer);
            callback();
        }
    }, 50);
}

function suitInfo(card) {
    if (card.isJoker) return { name: 'joker', symbol: '🃏' };
    return SUITS[card.suit] || SUITS[0];
}

function createCardElement(card) {
    const el = document.createElement('div');
    const info = suitInfo(card);
    el.className = 'card card-front ' + info.name;

    const rank = document.createElement('span');
    rank.className = 'rank';
    rank.textContent = card.isJoker ? 'Joker' : String(card.display).replace(/[♠♥♦♣]/g, '');

    const suit = document.createElement('span');
    suit.className = 'suit';
    suit.textContent = info.symbol;

    el.appendChild(rank);
    el.appendChild(suit);
    return el;
}

function render() {
    const state = Module.getGameState();

    document.getElementById('cpu-count').textContent = `(${Module.getCPUHandSize()}枚)`;
    document.getElementById('player-count').textContent = `(${Module.getPlayerHandSize()}枚)`;
    document.getElementById('message').textContent = Module.getMessage();

    const playerHandEl = document.getElementById('player-hand');
    playerHandEl.innerHTML = '';
    Module.getPlayerHand().forEach((card) => playerHandEl.appendChild(createCardElement(card)));

    const cpuHandEl = document.getElementById('cpu-hand');
    cpuHandEl.innerHTML = '';
    const isPlayerTurn = state === STATE.PLAYER_TURN;
    for (let i = 0; i < Module.getCPUHandSize(); i++) {
        const back = document.createElement('div');
        back.className = 'card card-back' + (isPlayerTurn ? '' : ' disabled');
        if (isPlayerTurn) {
            back.addEventListener('click', () => onDrawFromCPU(i));
        }
        cpuHandEl.appendChild(back);
    }

    const pile = document.getElementById('discard-pile');
    pile.innerHTML = '';
    Module.getDiscardedPairs().forEach((card) => pile.appendChild(createCardElement(card)));

    if (state === STATE.GAME_OVER) {
        showGameOver();
    } else if (state === STATE.CPU_TURN) {
        scheduleCpuTurn();
    }
}

function onDrawFromCPU(index) {
    if (Module.getGameState() !== STATE.PLAYER_TURN) return;
    Module.drawFromCPU(index);
    render();
}

function scheduleCpuTurn() {
    if (cpuTimer !== null) return;
    cpuTimer = setTimeout(() => {
        cpuTimer = null;
        Module.cpuTurn();
        render();
    }, 900);
}

function showGameOver() {
    const winner = Module.getWinner();
    const result = document.getElementById('result-message');
    result.textContent = winner === 0 ? 'あなたの勝ち！🎉' : 'CPUの勝ち… ババはあなたの手元に残りました';
    document.getElementById('game-over').classList.remove('hidden');
}

function startGame() {
    if (cpuTimer !== null) {
        clearTimeout(cpuTimer);
        cpuTimer = null;
    }
    Module.initGame();
    document.getElementById('start-screen').classList.add('hidden');
    document.getElementById('game-screen').classList.remove('hidden');
    document.getElementById('game-over').classList.add('hidden');
    render();
}

document.getElementById('start-btn').addEventListener('click', () => whenModuleReady(startGame));
document.getElementById('restart-btn').addEventListener('click', () => whenModuleReady(startGame));
