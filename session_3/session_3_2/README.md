# 3.2 完整門鎖電路
完成電門鎖與門窗感測器兩部分之後，就可以將它們組合起來，成為完整的電門鎖。在組合過程中，除了要注意可能會有所衝突外，也可能會因此產生新的功能。

## 線路圖
<img src="../imgs/session_3_2-schematic.png" width="300" alt="物聯網電門鎖完整線路圖" title="物聯網電門鎖完整線路圖">

## 麵包板
<img src="../imgs/session_3_2_breadboard.jpg" width="400" alt="物聯網電門鎖完整麵包板" title="物聯網電門鎖完整麵包板">

## 使用標籤來組織程式
假如將電門鎖和門窗感測的草稿碼合併成一個文件，草稿碼會變得很冗長雜亂，不利於找出錯處。所以最好是按功能將程式碼分拆成多個檔案，即建立函式庫，這就可以令草稿碼看起來很簡潔了。

在 Arduino IDE ，函式庫可以以標籤方式存在（另一個方式是放在 Arduino/libraries 目錄下，讓其他專案也可以取用）。所有標籤都會在驗證／上載時一同編譯。

<img src="../imgs/tab_view.jpg" width="400" alt="標籤" title="標籤" />

### 標頭檔（ .h ）與程式碼（ .cpp ）
分拆的程式碼要儲存在以`.cpp`為擴展名的檔案，與`.ino`草稿碼檔案放在同一個目錄裡。但`.cpp`檔必須要製作同名的`.h`**標頭檔**列出`.cpp`裡所有函式的**原型**。

標頭檔有幾個要點：

* 每個`.cpp`檔必須有同名的`.h`標頭檔，而`.h`標頭檔就不一定需要附有`.cpp`程式碼（參閱「[分拆細節](#分拆細節)」）；
* 在`.h`標頭檔宣告全域變數時，必須在前面加上`extern`關鍵字以標明變數會在其他地方設定初值，否則會被視為重覆宣告；
* 所有在`.cpp`檔裡的函數都必須在`.h`標頭檔裡宣告原型，而且呼叫方式必須一致；
* 所有全域變數的初值應在`.cpp`裡設定，不能與`extern`宣告放在一起；

### 建立標籤  

點擊草稿碼視窗右上角的「▼」菜單鍵，選擇「新增標籤」：

<img src="../imgs/insert_tab.jpg" width="250" alt="新增標籤" title="新增標籤" />

視窗下方顯示輸入欄位，在那裡輸入檔案的名稱，如「lock.cpp」：

（圖）

### 分拆細節

我們將草稿碼按功能分拆成 4 部分，共 6 個檔案：

* [主程式 session\_3\_2.ino](session_3_2.ino)。電門鎖程式的入口，負責控制程式流程；
* 電門鎖函式庫([lock.cpp](lock.cpp) | [lock.h](lock.h))。負責處理有關電門鎖的操作，包括開關電門鎖、處理電掣事件、自動重新鎖等；
* 門窗感測器函式庫([door.cpp](door.cpp) | [door.h](door.h))。負責處理門窗感測器的感測和反應。
* [設定檔 settings.h](settings.h)。只有一個`.h`標頭檔，集中存放整個電門鎖程式的設定值，例如 GPIO 針位、重新上鎖的時間等，將來還可以用來存放 Wi-Fi 密碼、伺服器證書等設定資料。

由於`lock.cpp`和`door.cpp`都有設定 GPIO 的 `gpioSetup()`，所以分拆時就要將它分別改名為`lockSetup()`和`doorSetup()`，在`session_3_2.ino`的`setup()`裡執行。

## 解決開著門時自動上鎖的問題

過去電門鎖有一個問題：就是有可能在未關上之前就自動上鎖，如果使用「撥閂型電門鎖」的話，這有可能會因為橫閂伸了出來而無法關門上鎖。整合了門窗感測之後，我們就可以按照門的關關狀況而決定甚麼時候自動上鎖。

要注意的是，由於使用磁石的關係，門關到一定程度時，門窗感測器就會以為門已關上。如果一感測到門關上後就立即上鎖的話，有可能會在未關貼前就上鎖，所以即使門窗感測器感應到門關上，也應該延遲一點以避免問題。

* 當`GPIO17`處於`HIGH`（解鎖）時，如果`GPIO16`同樣被設為`HIGH`（開門），即暫停自動重新上鎖；
* 當`GPIO16`回復為`LOW`（關門）後，延遲 1.5 秒後將`GPIO17`設為`LOW`
我們在 `settings.h` 裡新增一個常數 `AUTO_RELOCK_DELAY` 來設定延遲時間：

```cpp
#define AUTO_RELOCK_DELAY  1500
```

然後，將 `lock.cpp` 裡 `handleAutoRelock()` 函數的自動上鎖判斷式改成這樣：

```cpp
if (!isLocked && 
    !isDoorOpened && 
    (millis() - lastUnlockTime) > UNLOCK_TIMEOUT && 
    (millis() - lastDoorBounceTime + DOOR_BOUNCE_DELTA) > AUTO_RELOCK_DELAY
) {
    ...
}
```