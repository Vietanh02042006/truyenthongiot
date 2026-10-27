
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define KIEU_PHAN_CUNG   MD_MAX72XX::ICSTATION_HW
#define SO_IC            4
#define CHAN_LOAD        10
#define DO_SANG          8        // 0..15

MD_Parola manHinh = MD_Parola(KIEU_PHAN_CUNG, CHAN_LOAD, SO_IC);

/* -------- danh sach noi dung se hien lan luot -------- */
struct NoiDung {
  const char *chuoi;
  textEffect_t hieuUngVao;
  textEffect_t hieuUngRa;
  uint16_t     giuLai;      // thoi gian dung yen, tinh bang ms
};

NoiDung danhSach[] = {
  { "HELLO",                   PA_PRINT,       PA_NO_EFFECT,  2000 },
  { "TRUYEN THONG TRONG IOT",  PA_SCROLL_LEFT, PA_SCROLL_LEFT,   0 }
};

const byte SO_MUC = sizeof(danhSach) / sizeof(danhSach[0]);
byte mucHienTai = 0;

/* ======================= SETUP & LOOP =================================== */

void setup() {
  manHinh.begin();
  manHinh.setZoneEffect(0, true, PA_FLIP_UD);
  manHinh.setZoneEffect(0, true, PA_FLIP_LR);
  manHinh.setIntensity(DO_SANG);
  manHinh.displayClear();
  hienMucTiepTheo();
}

void loop() {
  // displayAnimate() tra ve true khi hieu ung hien tai da chay xong
  if (manHinh.displayAnimate()) {
    mucHienTai = (mucHienTai + 1) % SO_MUC;
    hienMucTiepTheo();
  }
}

/* ======================= HAM PHU ======================================== */

void hienMucTiepTheo() {
  NoiDung &m = danhSach[mucHienTai];
  manHinh.displayText(m.chuoi, PA_CENTER, 60, m.giuLai,
                      m.hieuUngVao, m.hieuUngRa);
}

