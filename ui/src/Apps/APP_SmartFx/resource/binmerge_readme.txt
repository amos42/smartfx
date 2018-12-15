
binary merge 툴이에요.

사용법은 다음과 같습니다.

BinMerge -o output [-a addr1] [-b blank-char1] file1 [-a addr2] [-b blank-char1] file2 [[-a addr3] [-b blank-char3] file3 [...]]

입력 파일 갯수는 제약이 따로 없습니다. 중간에 -a 파라미터를 주면 이후에 배치 주소가 바뀝니다. -b로 그 사이를 채워 줄 캐릭터 값을 정해 줍니다.

예를 들어, splash.bin과 res.bin을 합쳐서 res_full.bin을 만들고, res.bin은 0x10000 부터 배치하고자 하고, 그 빈 공간은 0xFF로 채워주고자 한다면 다음과 같이 입력합니다.

BinMerge -o ui_full.bin Scr_logo.img.bin -a 0x100000 -b 0xFF ui.bin

