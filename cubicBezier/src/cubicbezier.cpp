
//==================================================
// cubicBezier
//==================================================
#define BOOST_PYTHON_STATIC_LIB
#define BOOST_NUMPY_STATIC_LIB
#include <Python.h>
#include <valarray>
#include <iostream>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

/*
 ◆開発環境
 [OS]: macOS 10.14~
 [c++] gcc version 9.2.0
 [boost] 1.71.0
 [Python] v2.7.16
 */

typedef std::valarray<double> darray;
typedef std::valarray<darray> d2array;
namespace py = boost::python;
namespace cubicBezier
{
    darray cubicBezier(darray p0, darray p1, darray p2, darray p3, double u, bool restraint=true)
    {
        darray _p0 (2);
        darray _p1 (2);
        darray _p2 (2);
        darray _p3 (2);
        _p0 = p0; //# start-point
        _p1 = p1; //# start-point weighted
        _p2 = p2; //# end-point weighted
        _p3 = p3; //# end-point

        
        if(restraint)
        { // p0, p1, p2, p3のx値が互い違いになっている場合の為の処理。
            // p0[0]==3, p1[0]==1の様にp0がp1より先のx地点にある場合。
            double scale;
            double btKey;
            double vx_A;
            double vx_B;
            btKey = _p3[0] - _p0[0];
            vx_A = _p1[0] - _p0[0];
            vx_B = _p3[0] - _p2[0];
            scale = btKey/(vx_A + vx_B);
            if(scale<1)
            {
                darray vec_A (2);
                darray vec_B (2);
                vec_A = _p1 - p0;
                vec_B = _p2 - p3;
                //vecB*scale;
                
                _p1 = vec_A*scale; //vec_A apply ([=](double v) (return v*sclae;))
                _p2 = vec_B*scale; //vec_B apply ([=](double v) (return v;))
                
                _p1 = _p1+_p0;
                _p2 = _p2+_p3;
            }
        }
        darray _p4 (2);
        darray _p5 (2);
        darray _p6 (2);
        darray _p7 (2);
        darray _p8 (2);
        darray _p9 (2);
        
        // 計算
        _p4 = (_p1 - _p0)*u +_p0; //
        _p5 = (_p2 - _p1)*u +_p1; //
        _p6 = (_p3 - _p2)*u +_p2; //
        _p7 = (_p5 - _p4)*u +_p4; //
        _p8 = (_p6 - _p5)*u +_p5; //
        _p9 = (_p8 - _p7)*u +_p7; //
        return _p9;
    }

    d2array sampling_cubicBezier_t(d2array points, int count=100)
    {
    /*
     cubicBezierのポイント４点を用いて４点間のcubicBezierの離散化したカーブ(point群)を生成する。
     */
    double u;
    darray p;
    d2array rList(count);
    for(int t=0; t <count; t++)
        {
            u = double(t) / double(count-1);
            p = cubicBezier::cubicBezier(points[0], points[1], points[2], points[3], u);
            rList[t] = p;
        }
    return rList;
    }

}


//==================================================
// convert
//==================================================
d2array convert_list_to_d2array(py::list array)
{
    // array is expected as a d2array (referenced-code)
    int count = py::len(array);
    d2array rArray(count);
    py::list val;
    
    for(int i=0; i<count; i++)
        {
            val = py::extract<py::list>(array[i]);
            int mCount = py::len(val);
            rArray[i] = darray(mCount);
            for(int m=0; m<mCount; m++)
                {
                rArray[i][m] = py::extract<double>(val[m]);
                }
            
        }
    return rArray;
}

py::list convert_array_to_list(d2array array)
{
    // array is expected as a d2array //(referencerd-code)
    int count = array.size();
    py::list rArray;
    for(int i=0; i<count; i++)
        {
            py::list temp;
            int mCount = array[i].size();
            for(int m=0; m<mCount; m++)
                {
                temp.append(array[i][m]);
                }
            rArray.append(temp);
        }
    return  rArray;
}

//==================================================
// python command
//==================================================
py::list py_sampling_cubicBezier(py::list points, int count=100)
{
           d2array _points(count);
           _points = convert_list_to_d2array(points);
           d2array tmpArray;

           tmpArray = cubicBezier::sampling_cubicBezier_t(_points, count);
           return convert_array_to_list(tmpArray);
}

py::list py_cubicBezier(py::list p0, py::list p1, py::list p2, py::list p3, double u, bool restraint=true )
{
    darray _p0(2);
    darray _p1(2);
    darray _p2(2);
    darray _p3(2);
    _p0[0] = py::extract<double>(p0[0]);
    _p0[1] = py::extract<double>(p0[1]); //# start-point
    _p1[0] = py::extract<double>(p1[0]);
    _p1[1] = py::extract<double>(p1[1]); //# start-point(weighted)
    _p2[0] = py::extract<double>(p2[0]);
    _p2[1] = py::extract<double>(p2[1]); //# end-point(weigted)
    _p3[0] = py::extract<double>(p3[0]);
    _p3[1] = py::extract<double>(p3[1]); //# end-point

    darray points = cubicBezier::cubicBezier(_p0, _p1, _p2, _p3, u, restraint);
    py::list rValue;
    for(auto& v:points)
       {
        rValue.append(v);
       }
    return rValue;
}

BOOST_PYTHON_MODULE(lib)
{
           def("cubicBezier", &py_cubicBezier);
           def("sampling_cubicBezier", &py_sampling_cubicBezier);
}
