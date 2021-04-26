/*NumberWithUnits& operator+=(const NumberWithUnits& other) {
            bunit.unit_num+= other.bunit.unit_num;
            sunit.unit_num+= other.sunit.unit_num;
            return *this; // for call chaining
        }*/
#include<iostream>
#include<string>
#include <fstream>
#include <map>
#include<utility>
#include <cfloat>
#include <algorithm>
#include "NumberWithUnits.hpp"
using namespace std;
using namespace ariel;

static std::map<std::string,std::map<std::string,double>> units;
const double eps = 0.0001;

static bool find_type(const string& type)
{    
    try
    {
        units.at(type);
        return true;
    }
    catch(const std::exception& e)
    {
        return false;
    }
}

    
static double check_legal_types(const string& type1,const string& type2)
{
    if(type1==type2)
    {
        return 1;
    }
    try
    {
        return units.at(type1).at(type2);           
    }
    catch(const std::exception& e)
    {
        return -1;
    }
    
}

static void undirect_convert()
{
    
    for(auto& i: units)
    {
        string type1=i.first;
        for(auto& j: units.at(type1))
        {
            string type2=j.first;
            double ratio12=units.at(type1).at(type2);
            for(auto& k: units.at(type2))
            {
                string type3=k.first;
                if (type1!=type3)
                {
                    if(check_legal_types(type1,type3)==-1)
                    {    
                        double ratio23=units.at(type2).at(type3);
                        units[type1][type3]=ratio12*ratio23;
                        units[type3][type1]=1/(ratio12*ratio23);
                    }
                }

            }
        }
    }
}


NumberWithUnits::NumberWithUnits(double num, const string& unit) 
{
    if(!find_type(unit))
    {
       
        throw invalid_argument("invalid type");
    }
    unit_num=num;
    unit_type=unit;

}


NumberWithUnits::NumberWithUnits(double num,  const char* unit) 
{

    if(!find_type(unit))
    {
        throw invalid_argument("invalid type");
    }
    string unit_t{unit};
    unit_num=num;
    unit_type=unit_t;
}



static bool get_check_nxt_char(ifstream& input, char expectedChar) 
{
    char actualChar{};
    input >> actualChar;
    return (actualChar==expectedChar);
}

void NumberWithUnits::read_units(ifstream& units_file)
{
    ios::pos_type startPosition = units_file.tellg();
    double ratio=0;
    string type1{};
    string type2{};
    while(!units_file.eof())
    {  
        get_check_nxt_char(units_file,'1');
        units_file>>type1;
        get_check_nxt_char(units_file,'=');
        units_file>>ratio;
        units_file >> type2; 
        double excepted= check_legal_types(type1,type2);
        if(excepted==-1)
        {
            units[type1][type2]=ratio;
            units[type2][type1]=1/ratio;
        }
        else if(excepted!=ratio)
        {
            throw invalid_argument("duplicate pair with a diffrent ratio");
        }
    }
    
    undirect_convert();
}



NumberWithUnits ariel::operator+(const NumberWithUnits& u1,const NumberWithUnits& u2)
{
    double ratio=0;
    if(u1.unit_type == u2.unit_type)
    {
        return NumberWithUnits(u1.unit_num+u2.unit_num,u1.unit_type);
    }
    if((ratio =check_legal_types(u1.unit_type,u2.unit_type))!=-1)
    {

        double result_num=(u2.unit_num/ratio) +u1.unit_num;
        return NumberWithUnits(result_num,u1.unit_type);

    }
    throw invalid_argument("invalid types");

}
NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& other)
{
    double ratio=0;
    if(unit_type == other.unit_type)
    {
        unit_num+=other.unit_num;
        return *this;
    }
    if((ratio =check_legal_types(unit_type,other.unit_type))!=-1)
    {

        unit_num+=other.unit_num/ratio;
        return *this;

    }
    throw invalid_argument("invalid types");

}

NumberWithUnits& NumberWithUnits::operator++()
{
    unit_num++;
    return *this;
}

NumberWithUnits NumberWithUnits::operator+() const
{
    NumberWithUnits num(*this);
    return num ;
}

NumberWithUnits NumberWithUnits::operator++(int dummy_flag_for_postfix_increment)
{
    NumberWithUnits copy(*this);
    unit_num++;
    return copy ;
}

NumberWithUnits ariel::operator-(const NumberWithUnits& u1,const NumberWithUnits& u2)
{
    double ratio=0;
    if(u1.unit_type == u2.unit_type)
    {
        return NumberWithUnits(u1.unit_num-u2.unit_num,u1.unit_type);
    }

    if((ratio =check_legal_types(u1.unit_type,u2.unit_type))!=-1)
    {

            double result_num=u1.unit_num-(u2.unit_num/ratio) ;
            return NumberWithUnits(result_num,u1.unit_type);
    }
    throw invalid_argument("invalid types");
}

NumberWithUnits& NumberWithUnits::operator-=(const NumberWithUnits& other)
{
    double ratio=0;
    if(unit_type == other.unit_type)
    {
        unit_num-=other.unit_num;
        return *this;
    }
    if((ratio =check_legal_types(unit_type,other.unit_type))!=-1)
    {

            unit_num-=other.unit_num/ratio;
            return *this;

    }
    throw invalid_argument("invalid types");
}

NumberWithUnits& NumberWithUnits::operator--()
{
    unit_num--;
    return *this;
}

NumberWithUnits NumberWithUnits::operator-() const
{
    return NumberWithUnits(-unit_num,unit_type);
}

NumberWithUnits NumberWithUnits::operator--(int dummy_flag_for_postfix_increment)
{
    NumberWithUnits copy(*this);
    unit_num--;
    return copy ;
}

bool ariel::operator==(const NumberWithUnits& u1,const NumberWithUnits& u2)
{
    double ratio=0;
    if(u1.unit_type == u2.unit_type)
        {
            return (abs(u1.unit_num-u2.unit_num)<=eps);
        }
    if((ratio =check_legal_types(u1.unit_type,u2.unit_type))!=-1)
    {
        if (ratio<1)
        {
            return (abs(ratio*u1.unit_num-u2.unit_num)<=eps);
        }
        return (abs(u1.unit_num-(1/ratio)*u2.unit_num)<=eps);

    }
        throw invalid_argument("invalid types");
}

bool ariel::operator!=(const NumberWithUnits& u1,const NumberWithUnits& u2)
{
    return !(u1==u2);
}

bool ariel::operator>(const NumberWithUnits& u1,const NumberWithUnits& u2)
{
    double ratio=0;
    if(u1.unit_type == u2.unit_type)
    {
            return (u1.unit_num>u2.unit_num);
    }
    if((ratio =check_legal_types(u1.unit_type,u2.unit_type))!=-1)
    {
        if (ratio<1)
        {
            return (ratio*u1.unit_num>u2.unit_num);
        }
        return (u1.unit_num>(1/ratio)*u2.unit_num);

    }
    throw invalid_argument("invalid types");
}

bool ariel::operator>=(const NumberWithUnits& u1,const NumberWithUnits& u2)
{
    double ratio=0;
    if(u1.unit_type == u2.unit_type)
        {
            return (abs(u1.unit_num-u2.unit_num)<=eps || u1.unit_num>u2.unit_num);
        }
    if((ratio =check_legal_types(u1.unit_type,u2.unit_type))!=-1)
    {
        if (ratio<1)
        {
            return (abs(ratio*u1.unit_num-u2.unit_num) <= eps || ratio*u1.unit_num>u2.unit_num);
        }
        return (abs(u1.unit_num-(1/ratio)*u2.unit_num)<=eps || u1.unit_num>(1/ratio)*u2.unit_num);
    }
    throw invalid_argument("invalid types");
}

bool ariel::operator<(const NumberWithUnits& u1,const NumberWithUnits& u2)
{
    double ratio=0;
    if(u1.unit_type == u2.unit_type)
        {
            return (u1.unit_num<u2.unit_num);
        }
    if((ratio =check_legal_types(u1.unit_type,u2.unit_type))!=-1)
    {
        if (ratio<1)
        {
            return (ratio*u1.unit_num<u2.unit_num);
        }
        return (u1.unit_num<(1/ratio)*u2.unit_num);
    }
        throw invalid_argument("invalid types");
}

bool ariel::operator<=(const NumberWithUnits& u1,const NumberWithUnits& u2)
{
    double ratio=0;
    if(u1.unit_type == u2.unit_type)
        {
            return(abs(u1.unit_num-u2.unit_num)<=eps || (u1.unit_num<u2.unit_num));
        }
    if((ratio =check_legal_types(u1.unit_type,u2.unit_type))!=-1)
    {
        if (ratio<1)
        {
            return (abs(ratio*u1.unit_num-u2.unit_num)<=eps || (ratio*u1.unit_num<u2.unit_num));
        }
        return (abs(u1.unit_num-(1/ratio)*u2.unit_num)<=eps ||(u1.unit_num<(1/ratio)*u2.unit_num));
    }

    throw invalid_argument("invalid types");
}

NumberWithUnits NumberWithUnits::operator* (double x)
{
    return NumberWithUnits(unit_num*x,unit_type);
} 
NumberWithUnits ariel::operator*(double x, const NumberWithUnits& n)
{
    return NumberWithUnits(n.unit_num*x,n.unit_type);
}
std::ostream& ariel::operator<< (std::ostream& os, const NumberWithUnits& n)
{
    return (os<<n.unit_num<<'['<<n.unit_type<<']');
}

static bool get_check_nxt_char(istream& input, char expectedChar) 
{
    char actualChar{};
    input >> actualChar;
    return (actualChar==expectedChar);

}

static bool get_type_check_closer(istream& input,string& type) 
{
    string str{};
    input>>str;
    size_t i=0;
    if((i=str.find(']'))!=string::npos)
    {
        type=str.substr(0,i);
        return find_type(type);
    }
    type=str;
    return (find_type(type)&& get_check_nxt_char(input,']'));
}


std::istream& ariel::operator>> (std::istream& is, NumberWithUnits& n)
{

double unit_num=0;
string unit_type{};   
if ( (!(is >> unit_num))                 ||
        (!(get_check_nxt_char(is,'[')))  ||
        (!(get_type_check_closer(is,unit_type))))
        
{
    throw invalid_argument("invalid input");
}
    n.unit_num=unit_num;
    n.unit_type=unit_type;

return is;
}    





