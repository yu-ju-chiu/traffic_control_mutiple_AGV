# traffic_control_mutiple_AGV

#### 環境依賴

VS code v1.57.1
C/C++ms-vscode.cpptools v1.5.0-insiders

#### 部署步驟
1. 以matlab(`get_map.m`)轉換地圖得到`地圖.txt`(此範例為`factory_map.txt`)
2. 當前資料夾開啟terminal
3. `$ make` traffic_control_mutiple_AGV.out
4. `$./traffic_control_mutiple_AGV.out`
5. 即有執行檔

#### V1.0.0 版本內容更新

1. 地圖簡單可視化
2. 使用者可輸入變數
3. 尋找路徑方式為(TWA)Time Window Algorism
4. 輸出csv檔顯示路徑（座標點與時間）

--
