#include <iostream>
#include <list>
#include <vector>
#include <cassert>
#define MAX_ORDER 6
#define PGSIZE 4096
namespace my
{

    class ppage_alloc
    {
    private:
        std::vector<::std::list<int>> free_area_;

    public:
        ppage_alloc() : free_area_(MAX_ORDER, std::list<int>())
        {
        }

        void add(int order, int start_address)
        {
            assert(order <= MAX_ORDER && order >= 0);
            free_area_[order].push_back(start_address);
        }
        int get_pages(int num)
        {
            assert(num <= 32 && num >= 0);
            int target_order = 0;
            int tmp_num = num;
            while (tmp_num)
            {
                tmp_num /= 2;
                target_order++;
            }
            int address;
            for (int i = target_order; i < MAX_ORDER; i++)
            {
                if (free_area_[i].empty())
                {
                    continue;
                }
                address = free_area_[i].front();
                free_area_[i].pop_front();
                realloc_rest_page(address + num * PGSIZE, i, mul(2, i) - num);
                return address;
            }
            return -1;
        }
        void display_free_area() const noexcept
        {
            int cur_pos = 0;
            for (const auto &l : free_area_)
            {
                std::cout << cur_pos << ": ";
                for (const auto &addr : l)
                {
                    std::cout << addr << " ";
                }
                std::cout << std::endl;
            }
        }
        ~ppage_alloc() = default;

    private:
        void realloc_rest_page(int address, int cur_pos, int num)
        {
            for (int i = cur_pos - 1; i >= 0; i--)
            {

                if (num >= mul(2, i))
                {
                    num -= mul(2, i);
                    free_area_[i].push_back(address);
                    address += mul(2, i) * PGSIZE;
                }
            }
        }
        int mul(int n, int k)
        {
            int res = 1;
            for (int i = 0; i < k; i++)
            {
                res *= n;
            }
            return res;
        }
    };
}

int main()
{
    my::ppage_alloc allocator;
    allocator.add(5, 0);
    int address = allocator.get_pages(20);
    std::cout << address << std::endl;
    allocator.display_free_area();
}