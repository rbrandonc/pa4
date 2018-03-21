#include "GBitmap.h"
#include "GMatrix.h"
#include "MyShader.h"
#include <iostream>
using namespace std;

MyShader::MyShader(const GBitmap& bit, const GMatrix& mat) : local(mat), bitmap(bit) {}

// Return true iff all of the GPixels that may be returned by this shader will be opaque.
bool MyShader::isOpaque() {
//    bitmap.computeIsOpaque();
    return bitmap.isOpaque();
}

// The draw calls in GCanvas must call this with the CTM before any calls to shadeSpan().
bool MyShader::setContext(const GMatrix& ctm) {
//    cout << ctm[0] << " " << ctm[1] << " "<< ctm[2] << " "<< endl;
//    cout << ctm[3] << " "<< ctm[4] << " "<< ctm[5] << endl;
//    cout << "--------------------" << endl;

    GMatrix tmp;
    tmp.setConcat(ctm, local);
    if(tmp.invert(&inverse)) {
//        cout << inverse[0] << " " << inverse[1] << " "<< inverse[2] << " "<< endl;
//        cout << inverse[3] << " "<< inverse[4] << " "<< inverse[5] << endl;
//        cout << "--------------------" << endl;
        return true;
    }
    return false;
}


/**
 *  Given a row of pixels in device space [x, y] ... [x + count - 1, y], return the
 *  corresponding src pixels in row[0...count - 1]. The caller must ensure that row[]
 *  can hold at least [count] entries.
 */
void MyShader::shadeRow(int x, int y, int count, GPixel row[]) {

//    GPoint p = inverse.mapXY(x, y);
//
//    for (int i = 0; i < count; i++) {
//        row[i] = *bitmap.getAddr(pinX(p.fX), pinY(p.fY));
//        p.fX += inverse[GMatrix::SX];
//        p.fY += inverse[GMatrix::KY];
//    }



    const float dx = inverse[GMatrix::SX];
    const float dy = inverse[GMatrix::KY];
    GMatrix temp = *(new GMatrix());
    inverse.invert(&temp);

    // cout << count << endl;
    // cout << temp[0] << " " << temp[1] << " "<< temp[2] << " "<< endl;
    // cout << temp[3] << " "<< temp[4] << " "<< temp[5] << endl;
//    cout << "--------------------" << endl;

//    const GPixel array[] = { fP0, fP1 };
    for (int i = 0; i < count; ++i) {
        GPoint loc = temp.mapXY(x, y);
        row[i] = *bitmap.getAddr(pinX(loc.fX), pinY(loc.fY));

//        row[i] = array[((int)loc.fX + (int)loc.fY) & 1];
//        loc.fX += dx;
//        loc.fY += dy;
        x++;
    }
}

int MyShader::pinX(float x) {
    if(x < 0) return 0;
    if(x >= bitmap.width()-1) return bitmap.width() -1;
    return GRoundToInt(x);
}

int MyShader::pinY(float y) {
    if(y < 0) return 0;
    if(y >= bitmap.height()-1) return bitmap.height() -1;
    return GRoundToInt(y);
}

std::unique_ptr<GShader> GCreateBitmapShader(const GBitmap& bit, const GMatrix& localInv) {
    return std::unique_ptr<GShader>(new MyShader(bit, localInv));
}