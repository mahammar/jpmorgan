// Example program
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <ctime>
#include <cmath>


using namespace std;

map<string, tuple<char, float, float, float>> stock_exchange = 
{ 
	   {"TEA", make_tuple('C', 0, 0, 100)},
	   {"POP", make_tuple('C', 8, 0, 100)},
	   {"ALE", make_tuple('C', 23, 0, 60)},
	   {"GIN", make_tuple('P', 8, 2, 100)},
	   {"JOE", make_tuple('C', 13, 0, 250)}		   
};

map<string, multimap<time_t, tuple<unsigned int, char, float>>> trades;


float calculateDividendYield(const string& stock, const float price)
{
	if (stock_exchange.find(stock) == stock_exchange.end()) 
		throw logic_error("stock: "+stock+" not found");
	
	if (price == 0)
		throw logic_error("cannot calculate dividend yield");	
	
	// if Common
	if (get<0>(stock_exchange[stock]) == 'C') 
		return get<1>(stock_exchange[stock])/price;
		
	// if Preferred
	if (get<0>(stock_exchange[stock]) == 'P') 
		return get<2>(stock_exchange[stock])*0.01*get<3>(stock_exchange[stock])/price;	
}

float calculateRatioP2E(const string& stock, const float price)
{
	if (get<1>(stock_exchange[stock]) == 0)
		throw logic_error("cannot calculate p2e ratio for stock: "+stock);	
	
	return price/get<1>(stock_exchange[stock]);			
}

void recordTrade(const string& stock, const unsigned int quatity, const string& type, const float price)
{
	if (type != "buy" && type != "sell") 
		throw invalid_argument("indicator: " + type + " invalid, either buy or sell");
	
	char indicator = (type=="buy")?'B':'S';	
	
	trades[stock].insert(make_pair(time(nullptr), make_tuple(quatity, indicator, price)));
}

float CalculateVolumeWeightedStockPrice(const string& stock)
{	
	double numerator = 0;
	long long unsigned int denumerator = 0;
	
	multimap<time_t, tuple<unsigned int, char, float>>::reverse_iterator rit;  //start with the more recent trades 
	for (rit=trades[stock].rbegin(); rit!=trades[stock].rend(); ++rit)
	{
		// only trades in past 15 minutes
		if (rit->first - time(nullptr) > 15*60 )
			break;
			
		numerator += get<0>(rit->second)*get<2>(rit->second);  // SUM of Pi*Qi
		denumerator += get<0>(rit->second);  // SUM of Qi
	}
	
	if (denumerator == 0)
		throw logic_error("cannot calculate dividend yield");	

	return  numerator/denumerator;
}

float CalculateAllShareIndexGBCE()
{
	vector<double> prices;
	for (auto stock : trades)
		for (auto transaction : stock.second)
		{
			if (get<2>(transaction.second) == 0)  // if one of the prices is 0
				return 0;
			prices.push_back(get<2>(transaction.second));	
		}
		
	// calculate the geometric mean
	double result = 1;
	unsigned int n = prices.size();	
	for(auto x : prices )
	{
		result *= pow(x,(double)1/n);	// not optimised, but avoid overflow
	}
	return result;
}



int main()
{
 
	int choice;		   
	do
	{
 		try
		{   	
		 	cout << endl;
			cout << "Options Menu\n";
		 	cout << "Please make your selection\n";
		 	cout << "1 - calculate dividend yield\n";
		 	cout << "2 - calculate P/E ratio\n";
		 	cout << "3 - record a trade\n";
		 	cout << "4 - calculate volume weighted stock\n";
		 	cout << "5 - calculate the GBCE all share index\n";
		 	cout << "0 - Exit\n";
		 	cout << "Selection: ";
			cin >> choice;
			if (choice != 5)
				cout << "Enter Parameters: ";		
			
			string stock;
			float price;
			unsigned int quantity;
			string type;			
			
			switch (choice) 
			{
				case 1: 
						cin >> stock >> price;
						transform(stock.begin(), stock.end(), stock.begin(), ::toupper);
						cout << "result: " << calculateDividendYield(stock, price) << endl;
						break;
					
				case 2: 
						cin >> stock >> price;
						transform(stock.begin(), stock.end(), stock.begin(), ::toupper);
						cout << "result: " << calculateRatioP2E(stock, price) << endl;
						break;
						
				case 3: 
						cin >> stock >> quantity >> type >> price;
						transform(stock.begin(), stock.end(), stock.begin(), ::toupper);
						transform(type.begin(), type.end(), type.begin(), ::tolower);
						recordTrade(stock, quantity, type, price);
						cout << "done!" << endl;
						break;
				
				case 4: 
						cin >> stock;
						transform(stock.begin(), stock.end(), stock.begin(), ::toupper);
						cout << "result: " << CalculateVolumeWeightedStockPrice(stock) << endl;
						break;
						
				case 5: 
						cout << "result: " << CalculateAllShareIndexGBCE() << endl;
						break;	
	
				default: 
						break;
			}
		}
	   	catch (const exception& e) 
		{
			cerr << e.what() << endl;
		}

	} while (choice != 0);
	
	return EXIT_SUCCESS;

}



/*
cout << calculateDividendYield("GIN", 500) << endl;
cout << calculateRatioP2E("GIN", 500) << endl;

recordTrade("GIN", 10,"buy",30);
recordTrade("GIN", 20,"buy",40);
recordTrade("GIN", 30,"sell",50);
recordTrade("GIN", 40,"buy",60);

cout << CalculateVolumeWeightedStockPrice("GIN") << endl;				  
cout << CalculateAllShareIndexGBCE() << endl;

*/
