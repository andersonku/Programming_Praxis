#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <iterator>
#include <stack>
#include <set>

template <class C> auto begin(C& c) -> decltype(c.begin());
template <class C> auto begin(const C& c) -> decltype(c.begin());
template <class C> auto end(C& c) -> decltype(c.end());
template <class C> auto end(const C& c) -> decltype(c.end());
template <class T, size_t N> T* begin(T (&array)[N]);
template <class T, size_t N> T* end(T (&array)[N]);

template <class _Tp, size_t _Np>
_Tp*
begin(_Tp (&__array)[_Np])
{
    return __array;
}

template <class _Tp, size_t _Np>
_Tp*
end(_Tp (&__array)[_Np])
{
    return __array + _Np;
}

class TestHarness
{
public:
   TestHarness():testCount(0), failCount(0){};
   template <typename T>
   bool is(T a, T b)
   {
      ++testCount;
      std::cout << "Test " << testCount << ":";
      if (a != b)
      {
         ++failCount;
         std::cout << " Failed: result = " << a << ", expecting: " << b << ".\n";
         return false;
      }
      else
      {
         std::cout << " Passed.\n";
         return true;
      }
   }
   
   bool hasAllPassed(){
      if (failCount == 0)
      {
         std::cout << testCount << "/" << testCount << " tests passed!\n";
      }
      else
      {
         std::cout << failCount << "/" << testCount << " tests failed!\n";
      }
   }
private:
   int testCount;
   int failCount;
};

double calculate(const char* str)
{
   std::istringstream iss(str);
   std::stack<int> stack;
   for_each(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(),
            [&stack](std::string s)
            {
               int n = atoi(s.c_str());
               if (s.empty())
               {
                  std::cout << "Error! Empty String found!\n";
               }
               else if ((n == 0) && (s[0] != '0'))
               {
                  //std::cout << "Non numeric found\n";
                  int b = stack.top();
                  stack.pop();
                  int a = stack.top();
                  stack.pop();
                  switch(s[0])
                  {
                  case '+':
                     stack.push(a+b);
                     break;
                  case '-':
                     stack.push(a-b);
                     break;
                  case '*':
                     stack.push(a*b);
                     break;
                  case '/':
                     stack.push(a/b);
                     break;
                  default:
                     std::cout << "Error: unknown operator!\n";
                     exit(1);
                     break;
                  }
               }
               else
               {
                  stack.push(n);
               }   
            });

   return stack.top();
}

typedef std::pair<const char*, const double> TestData;

std::vector<TestData> td = {
   { "1 1 +", 2 },
   { "1 2 +", 3 },
   { "1 2 + 3 *", 9 }
};

std::vector<std::pair<int, std::string>> tdPrime = {
   { 1,  "" },
   { 2,  "2" },
   { 3,  "2 3" },
   { 4,  "2 3" },
   { 5,  "2 3 5" },
   { 30, "2 3 5 7 11 13 17 19 23 29" }
};

std::string sieve( const int n )
{
   if (n < 2){
      return "";
   }
   
   std::vector<int> sieve(n+1, 0);
   for (int x = 2; (x*x <= n); ++x)
   {
      for (int i = x*x; (i <= n) && sieve[x] == 0; i += x)
      {
         sieve[i] = 1; // sieved;
      }
   }

   std::stringstream result;
   result << 2;  
   for (int i = 3; i <= n; ++i)
   {
      if (sieve[i] == 0)
      {
         result << " " << i;
      }
   }
   return result.str();
}

int occupied_table[25];

template <typename T>
bool isInARow(T it, int increment, int goal)
{
   for (int i = 0; i < goal; ++i)
   {
      if(*it == 0)
      {
         return false;
      }
      std::advance(it, increment);
   }
   return true;
}

bool isWinner(const int *t, const int dim)
{
   for (auto it = t; it < t+dim*dim; it += dim)
   {
      if (isInARow(it, 1, dim))
      {
         return true;
      }
   }

   for (auto it = t; it < t+dim; ++it)
   {
      if (isInARow(it, dim, dim))
      {
         return true;
      } 
   }
   
   if (isInARow(t, dim+1, dim))
   {
      return true;
   } 
   
   if (isInARow(t+dim-1, dim-1, dim))
   {
      return true;
   } 
   
   return false;
}

void cast_test(int *t)
{
   int (*a)[5] = reinterpret_cast<int (*) [5]>(t);
   for(auto it= begin(*a); it != end(*a); ++it)
   {
      std::cout << *it;
   }   
}

void fill_table(int *t)
{
   for (int i = 0; i < 5; ++i)
   {
      for (int j = 0; j < 5; ++j)
      {
         #include <cstdlib>
         t[5*i+j] = 15*i + 1 + rand() % 15;
         //std::cout << t[5*i+j] << " ";
      }
      //std::cout << std::endl;
   }
}

void resetTable(int *t){
   for (int i = 0; i < 25; ++i)
   {
      t[i] = 0;
   }
   t[12] = 1;
}

int findNumber(int *t, int n)
{
   for (int i = 0; i < 25; ++i)
   {
      if (n == t[i])
      {
         return i;
      }
   }
   return -1;
}

int pickNumber(std::set<int> &s)
{
   int myPick = 1 + rand() % 75;
   while (s.find(myPick) != s.end()){
      myPick = 1 + rand() % 75;
   }
   s.insert(myPick);
   return myPick;
}

int bingoSingle()
{
   int check[25];
   int numbers[25];
   std::vector<int> winningSteps;
   for (int i = 0; i < 100; ++i)
   {
      // pick numbers
      resetTable(check);
      fill_table(numbers);
      std::set<int> calledNumber;
      
      
      for (int j = 0; j < 75; ++j)
      {
         int numberPicked = pickNumber(calledNumber);
         std::cout << numberPicked << " ";
         // Mark table
         int address = findNumber(numbers, numberPicked);
         if (address != -1)
         {
            check[address] = 1;
         }
         if (isWinner(check, 5))
         {
            std::cout << "Found! in step: " << j << std::endl;
            winningSteps.push_back(j);
            break;
         }
      }
      std::cout << std::endl;
   }
   int sum = 0;
   for_each(winningSteps.begin(), winningSteps.end(), [&sum](int &i) {
         sum += i;
      });
   std::cout << "Average calls to win is:" << sum/100 << std::endl;
}

int bingoMulti()
{
   std::vector<int*>checks;
   std::vector<int*>numbers;
   for (int i = 0; i < 500; ++i)
   {
      checks.push_back(new int[25]);
      numbers.push_back(new int[25]);
   }
   std::vector<int> winningSteps;

   for (int i = 0; i < 100; ++i)
   {
      // pick numbers
      for_each(checks.begin(), checks.end(), [](int* t){
            resetTable(t);
         });
      for_each(numbers.begin(), numbers.end(), [](int* t){
            fill_table(t);
         });
      std::set<int> calledNumber;
            
      for (int j = 0; j < 75; ++j)
      {
         int numberPicked = pickNumber(calledNumber);
         std::cout << numberPicked << " ";
         // Mark table

         for (int k = 0; k < 500; ++k)
         {
            int address = findNumber(numbers[k], numberPicked);
            if (address != -1)
            {
               checks[k][address] = 1;
            }            
         }

         bool found = false;
         for_each(checks.begin(), checks.end(), [&found](int* t){
               if (isWinner(t, 5))
               {
                  found = true;
               }
         });
         if (found)
         {
            std::cout << "Found! in step: " << j << std::endl;
            winningSteps.push_back(j);
            break;
         }
      }
      std::cout << std::endl;
   }
   int sum = 0;
   for_each(winningSteps.begin(), winningSteps.end(), [&sum](int &i) {
         sum += i;
      });
   std::cout << "Average calls to win is:" << sum/100 << std::endl;
}

int josephus(int size, int gap)
{
   std::vector<int> group;
   for (int i = 0; i < size; ++i)
   {
      group.push_back(i);
   }

   int n = 0;
   while(group.size() > 1)
   {
      // kill kill kill
      n += 3;
      n %= group.size();
      auto it = group.begin() + n;
      group.erase(it);
      if (n == 0)
      {
         n = group.size() - 1;
      }
      else{
         n -= 1;
      }
      for_each( group.begin(), group.end(), [](int &n){
            std::cout << n << " ";
         });
      std::cout << std::endl;
   }
   return group[0];
   
}


int main(int argc, char *argv[])
{
   // Grab input
   // Create stack
   std::vector<double> stack;

   TestHarness th;
   // for_each(td.begin(), td.end(), [&th](TestData& i) {
   //       th.is( calculate(i.first), i.second);
   //    });

   // for_each (tdPrime.begin(), tdPrime.end(), [&th](std::pair<int, std::string> &i) {
   //       th.is(sieve(i.first), i.second);
   //       std::cout << "Primes from 1 to " << i.first << " : " << sieve(i.first) << std::endl;
   //    });
   // th.hasAllPassed();

   
   // int test[5]= {1,2,3,4,5};
   // cast_test(test);

   int victory_test[9] = {
      0, 0, 1,
      1, 1, 0,
      1, 0, 1,
   };

   bingoSingle();
   
   //bingoMulti();

   //std::cout << josephus(41,3) << std::endl;
   
   return 0;

   
}
