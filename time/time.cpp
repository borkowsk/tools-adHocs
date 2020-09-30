/// Duration example
// https://stackoverflow.com/questions/19555121/how-to-get-current-timestamp-in-milliseconds-since-1970-just-the-way-java-gets
// https://stackoverflow.com/questions/41077377/how-to-get-current-time-in-milliseconds/41077420
// https://en.cppreference.com/w/cpp/chrono/duration
// https://man7.org/linux/man-pages/man2/gettimeofday.2.html
///
#include <chrono>
#include <ctime>
#include <cstdint>
#include <iostream>
#include <sys/time.h> //unix standard

uint64_t timeSinceEpochMillisec() 
{
  using namespace std::chrono;
  
  auto tp=system_clock::now();
  std::time_t tt = std::chrono::system_clock::to_time_t(tp);
  std::tm tm = std::tm{0};
  gmtime_r(&tt, &tm);
  std::cout << tm.tm_year+1900 << '-';// Output year
  std::cout << tm.tm_mon + 1 << '-';// Output month
  std::cout << tm.tm_mday << ' ';// Output day
  // Output hour
  if (tm.tm_hour <= 9 )
        std::cout << '0';
  std::cout << tm.tm_hour << ':';

  // Output minute
  if (tm.tm_min <= 9)
        std::cout << '0';  
  std::cout << tm.tm_min << ':';
  
  // Output seconds with fraction
  //   This is the heart of the question/answer.
  //   First create a double-based second
  std::chrono::duration<double> sec = tp - 
                                    std::chrono::system_clock::from_time_t(tt) +
                                    std::chrono::seconds(tm.tm_sec);
  //   Then print out that double using whatever format you prefer.
  if (sec.count() < 10)
        std::cout << '0';
  std::cout << std::fixed << sec.count() << '\n';

  std::cout<<std::endl;

  auto tmp=tp.time_since_epoch();
  std::cout << "  hours:\t" << duration_cast<hours>  (tmp).count() << std::endl;
  std::cout << " minuts:\t" << duration_cast<minutes>(tmp).count() << std::endl;
  std::cout << "seconds:\t" << duration_cast<seconds>(tmp).count() << std::endl;
  return duration_cast<milliseconds>(tmp).count();
}

int timeUnix()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);//unix standard
    
    long long mslong = (long long) tp.tv_sec * 1000L + tp.tv_usec / 1000; //get current timestamp in milliseconds
    std::cout << "   sec.:\t" << tp.tv_sec << std::endl;
    std::cout << " +usec.:\t" << tp.tv_usec << std::endl;    
    std::cout << " =msec.:\t" << mslong << std::endl;
}

int main() 
{
  std::cout <<"C++11:" << std::endl;
  auto mili=timeSinceEpochMillisec();
  std::cout << "milisec:\t"<< mili << std::endl <<"\nUnix:\n";
  timeUnix();
  return 0;
}
