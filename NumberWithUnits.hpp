#pragma once
#include<iostream>
#include<string>
#include<map>
#include <unordered_map>
#include<vector>
namespace ariel{

     typedef class NumberWithUnits{

        private:
            double unit_num;
            std::string unit_type;
           // static inline std::map<std::pair<std::string,std::string>,double> r_units;
            
            
        public:
            NumberWithUnits(double num,const char  * unit);
            NumberWithUnits(double num,const std::string& unit);
            static void read_units(std::ifstream& units_file);
            friend NumberWithUnits operator+(const NumberWithUnits& u1,const NumberWithUnits& u2);
            NumberWithUnits& operator+=(const NumberWithUnits& other);
            NumberWithUnits& operator++();
            NumberWithUnits operator+() const;
            NumberWithUnits operator++(int dummy_flag_for_postfix_increment);
            

            friend  NumberWithUnits operator-(const NumberWithUnits& u1,const NumberWithUnits& u2);
            NumberWithUnits& operator-=(const NumberWithUnits& other);
            NumberWithUnits& operator--();
            NumberWithUnits operator-() const;
            NumberWithUnits operator--(int dummy_flag_for_postfix_increment);

            friend bool operator!=(const NumberWithUnits& u1,const NumberWithUnits& u2);
            friend bool operator>(const NumberWithUnits& u1,const NumberWithUnits& u2);
            friend bool operator>=(const NumberWithUnits& u1,const NumberWithUnits& u2);
            friend bool operator<(const NumberWithUnits& u1,const NumberWithUnits& u2);
            friend bool operator<=(const NumberWithUnits& u1,const NumberWithUnits& u2);
            friend bool operator==(const NumberWithUnits& u1,const NumberWithUnits& u2);

            NumberWithUnits operator* (double x); 

            friend NumberWithUnits operator*(double x, const NumberWithUnits& n);
            friend std::ostream& operator<< (std::ostream& os, const NumberWithUnits& n);
            friend std::istream& operator>> (std::istream& is, NumberWithUnits& n);



    } nwu ;

}