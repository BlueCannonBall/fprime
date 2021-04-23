#include <iostream>
#include <vector>
#include <thread>
#include <boost/program_options.hpp>
#include <sstream>

using namespace std;
using namespace boost::program_options;

vector<unsigned int> primes;
vector<thread> threads;

bool is_prime(unsigned int num) {
    if (num == 2 || num == 3) {
        return true;
    } else if (num == 0 || num == 1) {
        return false;
    } else if (num%2 == 0 || num%3 == 0) {
        return false;
    }

    unsigned int i = 5;
    while (i*i <= num) {
        if (num%i == 0 || num%(i + 2) == 0) {
            return false;
        }
        i += 6;
    }
    return true;
}

void find_primes(unsigned start_num, unsigned end_num, vector<unsigned int>* arr) {
    if (start_num%2 == 0) {
        if (start_num == 2 || start_num == 0) {
            arr->push_back(2);
        }
        start_num += 1;
    }

    for (unsigned int i = start_num; i<end_num; i+=2) {
        if (is_prime(i)) {
            arr->push_back(i);
        }
    }
}

void threaded_find_primes(unsigned start_num, unsigned end_num, unsigned thread_count) {
    vector<vector<unsigned int>> ranges;
    vector<vector<unsigned int>*> prime_arrays;
    unsigned int chunk_size = (end_num - start_num) / thread_count;
    for (unsigned int thread = 0; thread<thread_count; thread++) {
        vector<unsigned int> range;
        if (thread == thread_count - 1) {
            range = {start_num + (chunk_size * thread), end_num};
        } else {
            range = {start_num + (chunk_size * thread), start_num + (chunk_size * (thread + 1))};
        }
        ranges.push_back(range);
    }
    for (const auto& range : ranges) {
        prime_arrays.push_back(new vector<unsigned int>);
        threads.push_back(thread(find_primes, range[0], range[1], prime_arrays[prime_arrays.size() - 1]));
    }
    for (unsigned int thread = 0; thread<thread_count; thread++) {
        threads[thread].join();
    }
    for (const auto arr : prime_arrays) {
        for (unsigned int prime : *arr) {
            primes.push_back(prime);
        }
    }
}

void show_help(options_description& desc, const char *argv[]) {
    cout << "Usage: " << argv[0] << " [options] start_num end_num\n";
    cout << desc;
}

int main(int argc, const char *argv[]) {
    options_description desc{"Options"};
    variables_map vm;
    try {
        unsigned int start_num;
        unsigned int end_num;
        unsigned int thread_count = 8;

        desc.add_options()
            ("help,h", "Help screen")
            ("start-num", value(&start_num)->required(), "Starting number")
            ("end-num", value(&end_num)->required(), "Ending number")
            ("threads,t", value(&thread_count), "Amount of threads")
        ;
        positional_options_description p;
        p.add("start-num", 1);
        p.add("end-num", 1);

        store(command_line_parser(argc, argv).
            options(desc).positional(p).run(), vm);
        notify(vm);

        if (vm.count("help")) {
            show_help(desc, argv);
            return 0;
        }
        
        threaded_find_primes(start_num, end_num, thread_count);

        stringstream ss;
        for (size_t i = 0; i < primes.size(); ++i) {
            if (i != 0)
                ss << "\n";
            ss << primes[i];
        }
        cout << ss.str();

        return 0;
    }
    catch (exception& e) {
        if (vm.count("help") || argc <= 1) {
            show_help(desc, argv);
            if (argc <= 1) {
                return 1;
            }
            return 0;
        } else {
            cerr << e.what() << endl;
        }
    }
}
