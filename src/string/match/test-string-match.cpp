#include <string>
#include <vector>
#include <iostream>
#include <sys/time.h>
#include <cmath>

class bf {
public:
    bf(const std::string& pat) : pat(pat) {
    }

    int search(const std::string& txt) const {
        int M = txt.size(), i = 0;
        int N = pat.size(), j = 0;
        for(; i <= M-N; i++) {
            if(match(txt, i))
                return i;
        }
        return -1;
    }

private:
    bool match(const std::string& txt, int idx) const {
        int M = txt.size(), i = idx;
        int N = pat.size(), j = 0;
        for(; i < M && j < N; i++, j++) {
            if(txt[i] != pat[j])
                return false;
        }
        return true;
    }

private:
    const std::string pat;
};

class bm
{
public:
    bm(const std::string& pat) : pat(pat){
        int N = pat.size();
        for(int i = 0; i < N; i++)
            right[pat[i]] = i;  
    }

    int search(const std::string& txt){
        int i, M = txt.size();
        int j, N = pat.size();
        int skip = 0; //用于标志i应该向右移动多少字符
        for(i = 0; i <= M-N; i+=skip){
            skip = 0;
            for(j = N - 1; j >= 0; j--){
                if(pat[j] != txt[i+j]){
                    skip = j - right[txt[i+j]];
                    if(skip < 1)
                        skip = 1;
                    break;
                }
            }
            if(skip == 0)
                return i;
        }
        return -1;
    }
private:
    const int R = 256;
    std::vector<int> right = std::vector<int>(R, -1);
    const std::string& pat;
};

class kmp{
public:
    kmp(const std::string& pat) : pat(pat){
        initDFA();
    }

    int search(const std::string& txt){
        int i, M = txt.size();
        int j, N = pat.size();
        for(i = 0, j = 0; i < M && j < N; i++)
            j = dfa[j][txt[i]];
        
        return j == N ? i - N : -1;
    }

private:
    void initDFA(){
        int N = pat.size();
        const int R = 256;
        dfa = std::vector<std::vector<int>>((N == 0 ? 1 : N), std::vector<int>(R, 0));
        int X = 0;
        dfa[0][pat[0]] = 1;
        for(int s = 1; s < N; s++){
            for(char c : pat)
                dfa[s][c] = dfa[X][c];
            dfa[s][pat[s]] = s+1;
            X = dfa[X][pat[s]];
        }
    }

private:
    std::vector<std::vector<int>> dfa;
    const std::string& pat;
};

template<typename string_match>
std::pair<std::vector<int>, double> test_match(string_match&& match, const std::vector<std::string*> txts) {
    struct timeval start, end;
    double duration = 0;
    gettimeofday(&start, NULL);
    std::vector<int> res;
    for(const auto& txt : txts) {
        match.search(*txt);
    }

    gettimeofday(&end, NULL);

    duration = ((end.tv_sec - start.tv_sec) * pow(10, 6) + (end.tv_usec - start.tv_usec)) / pow(10, 6);
    return {res, duration};
}

int main(){
    const std::string pat = "782";
    const int range = 96411;

    const int len =7000000;

    std::vector<std::string*> txts;
    for(int i = 0; i < len; i++) {
        auto t = (std::to_string(rand() % range));

        txts.push_back(new std::string(t));
    }

    auto bft = test_match(bf(pat), txts);
    auto bmt = test_match(bm(pat), txts);
    auto kmpt = test_match(kmp(pat), txts);

    std::cout << "bf " << bft.second << std::endl;
    std::cout << "bm " << bmt.second << std::endl;
    std::cout << "kmp " << kmpt.second << std::endl;

    if(bft.first == bmt.first && bft.first == kmpt.first){
        std::cout << "result is passed" << std::endl;
    }else {
        std::cout << "result is failed" << std::endl;
        int len = bft.first.size();
        for(auto i : bft.first)
            std::cout << i << " " ;
        std::cout << std::endl;

        for(auto i : bmt.first)
            std::cout << i << " " ;
        std::cout << std::endl;

        for(auto i : kmpt.first)
            std::cout << i << " " ;
        std::cout << std::endl;
    }
    
    // std::cout << "bf " << test_match(bf(pat), len, range).second << std::endl;
    // std::cout << "bm " << test_match(bm(pat), len, range).second << std::endl;
    // std::cout << "kmp " << test_match(kmp(pat), len, range).second << std::endl;

    return 0;
}