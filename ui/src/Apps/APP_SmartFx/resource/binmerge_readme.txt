
binary merge ���̿���.

������ ������ �����ϴ�.

BinMerge -o output [-a addr1] [-b blank-char1] file1 [-a addr2] [-b blank-char1] file2 [[-a addr3] [-b blank-char3] file3 [...]]

�Է� ���� ������ ������ ���� �����ϴ�. �߰��� -a �Ķ���͸� �ָ� ���Ŀ� ��ġ �ּҰ� �ٲ�ϴ�. -b�� �� ���̸� ä�� �� ĳ���� ���� ���� �ݴϴ�.

���� ���, splash.bin�� res.bin�� ���ļ� res_full.bin�� �����, res.bin�� 0x10000 ���� ��ġ�ϰ��� �ϰ�, �� �� ������ 0xFF�� ä���ְ��� �Ѵٸ� ������ ���� �Է��մϴ�.

BinMerge -o ui_full.bin Scr_logo.img.bin -a 0x100000 -b 0xFF ui.bin

