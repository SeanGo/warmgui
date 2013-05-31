#ifndef __interface_calculator_h__
#define __interface_calculator_h__

namespace WARMGUI {
class WARMGUI_API ICalculator
{
public:
    ICalculator(void);
    ~ICalculator(void);

    virtual void AppendNewData(dataptr pdata, size_t datasize) {}

};

EXPORT_STL_VECTOR(WARMGUI_API, ICalculator*)

typedef std::vector<ICalculator*> CalculatorArray;
typedef CalculatorArray::iterator CalIter;
typedef CalculatorArray::const_iterator CalConstIter;


}   //namespace WARMGUI

#endif //__interface_calculator_h__
