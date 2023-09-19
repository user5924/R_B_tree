// R_B_tree.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#include "strings.h"

struct elem
{
    char color;

    char* key;
    float data;

    elem* past;
    elem* left;
    elem* right;
};

class R_B_tree
{
public:
    R_B_tree()
    {
        start = NULL;
        number = 0;
    }

    struct answer
    {
        elem* ppt;
        char direct;
    };

    bool add(char* key, float data)
    {
        if (start == NULL)
        {
            start = create_elem('B', key, data);
        }else
        { 
            answer ans = find(key);

            if (ans.direct == 'N')
                return false;

            struct elem* buff = create_elem('R', key, data);
            create_wires(ans.ppt, buff, ans.direct);
            //if (ans.ppt == start)
             ////   cout << "ans.ppt == start\n";
            ///if (start->past == NULL)
                //cout << "start -> past == NULL\n";

            struct elem* curr = buff->past;
            

            while (1)
            {
                
                /*
                *    0  alright
                *    -1  left red
                *    1  two rights red
                *    2  right and left red 
                */
                int status = get_status(curr);
                
                struct answer past = get_past(curr);
                switch (status)
                {
                case -1:
                    //cout << "left red\n";
                    curr = right_ret(curr);
                    //if (past.ppt == NULL)
                     //   cout << "past.ppt == NULL\n";
                    create_wires(past.ppt, curr, past.direct);
                    break;

                case 1:
                    curr = left_ret(curr);
                    create_wires(past.ppt, curr, past.direct);
                    swap_color(curr);
                    break;

                case 2:
                    swap_color(curr);
                    break;
                }

                if (curr->past == NULL)
                {
                    //cout << "curr->past == NULL\n";
                    start = curr;
                    start->color = 'B';
                    break;
                }
                else
                    curr = curr->past;  
            }
        }

        number += 1;
        return true;
    }

    bool del(char* key)
    {
        if (start == NULL)
            return false;

        answer ans = find(key);
        if (ans.direct != 'N')
            return false;
        else
        {
            elem* to_del;

            if (ans.ppt->left != NULL)
            {
                to_del = ans.ppt->left;
                while (to_del->right != NULL)
                    to_del = to_del->right;
            }
            else
            {
                if (ans.ppt->right != NULL)
                    to_del = ans.ppt->right;
                else
                    to_del = ans.ppt;
            }

            swap(to_del->key, ans.ppt->key);
            swap(to_del->data, ans.ppt->data);
            //cout << "to_del = " << to_del->key << endl;

            if (to_del->color == 'R')
            {
                ans = get_past(to_del);
                if (ans.direct == 'L')
                    ans.ppt->left = NULL;
                else
                    ans.ppt->right = NULL;

                del_elem(to_del);
                cout << "to_del->color == 'R'\n";
            }
            else
            {
                int flag;
                elem* tail = NULL;
                ans = get_past(to_del);
                del_elem(to_del);

                while (1)
                {
                    if (ans.ppt == NULL)
                    {
                        start = tail;
                        cout << "start = tail\n";
                        break;
                    }
                    else
                    {
                        answer past = get_past(ans.ppt);
                        create_wires(ans.ppt, tail, ans.direct);
                        if(tail != NULL)
                            if (tail->color == 'R')
                            {
                                cout << "tail->color == 'R'\n";
                                exit(1);
                            }

                        if (ans.direct == 'L')
                        {
                            if (ans.ppt->color == 'B')
                            {
                                if (ans.ppt->right->color == 'R')
                                {
                                    flag = 1;
                                    elem* Z1 = ans.ppt->right->left;
                                    elem* curr;
                                    ans.ppt->right->right;
                                    if (Z1->right == NULL or (Z1->right->color == 'B'))
                                    {
                                        cout << "fir\n";
                                        curr = left_ret(ans.ppt);
                                        curr->left->color = 'B';
                                        curr->left->right->color = 'R';                                        
                                    }
                                    else
                                    {
                                        cout << "sec\n";
                                        elem* X, * Y, * Z1L, * Z1R, * Z2;

                                        X = ans.ppt;
                                        X->color = 'B';
                                        Y = ans.ppt->right;
                                        Y->color = 'R';
                                        Z1 = Y->left;
                                        Z1->color = 'B';
                                        Z1L = Z1->left;
                                        //Z1L->color = 'B';
                                        Z1R = Z1->right;
                                        if (Z1R != NULL)
                                            Z1R->color = 'B';
                                        //Z1R->color = 'B';
                                        Z2 = Y->right;
                                        Z2->color = 'B';

                                        create_wires(X, tail, 'L');
                                        create_wires(X, Z1L, 'R');
                                        create_wires(Y, Z1R, 'L');
                                        create_wires(Y, Z2, 'R');
                                        create_wires(Z1, X, 'L');
                                        create_wires(Z1, Y, 'R');

                                        curr = Z1;
                                        
                                    }

                                    if (past.ppt != NULL)
                                        create_wires(past.ppt, curr, past.direct, flag);
                                    else
                                        start = curr;

                                    cout << "CURR -> KEY = " << curr->key << endl;
                                    cout << "CURR -> LEFT = " << curr->left->key << endl;
                                    cout << "CURR -> RIGHT = " << curr->right->key << endl;
                                    cout << "CURR -> PAST = " << curr->past->key << endl;

                                    break;
                                }
                                else
                                {
                                    flag = 2;
                                    if (ans.ppt->right->right == NULL or
                                        (ans.ppt->right->right->color == 'B'))
                                    {
                                        tail = ans.ppt;
                                        ans.ppt->right->color = 'R';
                                        ans = past;
                                    }
                                    else
                                    {
                                        elem* curr = left_ret(ans.ppt);
                                        curr->left->color = 'B';
                                        curr->right->color = 'B';

                                        if (past.ppt != NULL)
                                            create_wires(past.ppt, curr, past.direct, flag);
                                        else
                                            start = curr;

                                        break;
                                    }
                                }
                            }
                            else
                            {
                                flag = 3;
                                if (ans.ppt->right->right == NULL or ans.ppt->right->right->color == 'B')
                                {
                                    cout << "first\n";
                                    ans.ppt->color = 'B';
                                    ans.ppt->right->color = 'R';
                                }
                                else
                                {
                                    cout << "second\n";
                                    elem* curr = left_ret(ans.ppt);
                                    curr->left->color = 'B';
                                    curr->right->color = 'B';

                                    if (past.ppt != NULL)
                                        create_wires(past.ppt, curr, past.direct, flag);
                                    else
                                        start = curr;
                                }

                                break;
                            }
                        }
                        else
                        {
                            if (ans.ppt->color == 'R')
                            {
                                flag = 4;
                                elem* curr;
                                if (ans.ppt->left->right == NULL or
                                    ans.ppt->left->right->color == 'B')
                                {
                                    curr = right_ret(ans.ppt);
                                    curr->color = 'B';
                                }
                                else
                                {
                                    elem* Y, * X, * Z1, * Z2, * Z2L, * Z2R;
                                    Y = ans.ppt;
                                    Y->color = 'B';
                                    X = Y->left;
                                    X->color = 'B';
                                    Z1 = X->left;
                                    Z2 = X->right;
                                    Z2->color = 'R';
                                    Z2L = Z2->left;
                                    Z2R = Z2->right;

                                    create_wires(X, Z1, 'L');
                                    create_wires(X, Z2L, 'R');
                                    create_wires(Y, Z2R, 'L');
                                    create_wires(Y, tail, 'R');
                                    create_wires(Z2, X, 'L');
                                    create_wires(Z2, Y, 'R');

                                    curr = Z2;
                                }

                                if (past.ppt != NULL)
                                    create_wires(past.ppt, curr, past.direct, flag);
                                else
                                    start = curr;

                                break;
                            }
                            else
                            {
                                flag = 5;
                                elem* curr;
                                if (ans.ppt->left->right == NULL or
                                    ans.ppt->left->right->color == 'B')
                                {
                                    ans.ppt->left->color = 'R';
                                    curr = right_ret(ans.ppt);
                                    curr->color = 'B';
                                    tail = curr;

                                    ans = past;
                                }
                                else
                                {
                                    elem* Y, * X, * Z1, * Z2, * Z2L, * Z2R;
                                    Y = ans.ppt;
                                    Y->color = 'B';
                                    X = Y->left;
                                    X->color = 'B';
                                    Z1 = X->left;
                                    Z2 = X->right;
                                    Z2->color = 'B';
                                    Z2L = Z2->left;
                                    Z2R = Z2->right;

                                    create_wires(X, Z1, 'L');
                                    create_wires(X, Z2L, 'R');
                                    create_wires(Y, Z2R, 'L');
                                    create_wires(Y, tail, 'R');
                                    create_wires(Z2, X, 'L');
                                    create_wires(Z2, Y, 'R');

                                    curr = Z2;
                                    if (past.ppt != NULL)
                                        create_wires(past.ppt, curr, past.direct, flag);
                                    else
                                        start = curr;
                                    break;
                                }
                            }
                        }
                    }
                }
     
            }
        }

        number -= 1;
        if(start != NULL)
             start->past = NULL;
        return true;
    }

    void post(fstream& f)
    {
        elem** stack;
        stack = new elem * [number];
       // cout << "number = " << number << endl;
        int st_ppt = 0;

        if (start == NULL)
        {
            f << "No data!\n";
            return;
        }

        elem* curr = start;
        bool fl_ret = false;

        while (1)
        {
            if (fl_ret == false and curr->left != NULL)
            {
                stack[st_ppt] = curr;
                st_ppt += 1;
                curr = curr->left;
            }
            else
            {
                f << curr->key << "    " << curr->data << "    " << curr->color << endl;

                if (curr->right != NULL)
                {
                    curr = curr->right;
                    fl_ret = false;
                }
                else
                {
                    st_ppt -= 1;
                    if (st_ppt < 0)
                        break;
                    else
                    {
                        fl_ret = true;
                        curr = stack[st_ppt];
                    }
                }
            }
        }

        f << endl;
        delete[] stack;
    }


    bool check_struct()
    {
        if (start == NULL)
            cout << "Alright!\n";
        else
        {
            elem** buff[2];
            buff[0] = new elem * [number];
            buff[0][0] = start;
            buff[1] = new elem * [number];
            int i, max_n = 1, max_f = 0;
            int now = 0, far = 1;

            while (1)
            {
                //cout << "Cuicle!\n";
                max_f = 0;

                for (i = 0; i < max_n; i += 1)
                {
                    if (buff[now][i]->left != NULL)
                        if (buff[now][i] != buff[now][i]->left->past)
                        {
                            cout << "At '" << buff[now][i]->key <<
                                "' left is not back correct\n";
                            return false;
                        }

                    if (buff[now][i]->right != NULL)
                        if (buff[now][i] != buff[now][i]->right->past)
                        {
                            cout << "At '" << buff[now][i]->key <<
                                "' right is not back correct\n";
                            return false;
                        }


                    if (buff[now][i]->color == 'B')
                    {
                        if (buff[now][i]->left == NULL)
                        {
                            if (buff[now][i]->right != NULL)
                                if (buff[now][i]->right->color == 'B')
                                {
                                    cout << "At '" << buff[now][i]->key <<
                                        "' color B, left is NULL, right is B\n";
                                    return false;
                                }
                                else
                                {
                                    buff[far][max_f] = buff[now][i]->right;
                                    max_f += 1;
                                }
                        }
                        else
                        {
                            if (buff[now][i]->left->color == 'R')
                            {
                                cout << "At '" << buff[now][i]->key <<
                                    "' color B, left is R\n";
                                return false;
                            }
                            else
                            {
                                if (buff[now][i]->right == NULL)
                                {
                                    cout << "At '" << buff[now][i]->key <<
                                        "' color B, left is R, right is NULL\n";
                                    return false;
                                }
                                else
                                {
                                    buff[far][max_f] = buff[now][i]->left;
                                    max_f += 1;
                                    buff[far][max_f] = buff[now][i]->right;
                                    max_f += 1;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (buff[now][i]->left == NULL)
                            if (buff[now][i]->right != NULL)
                            {
                                cout << "At '" << buff[now][i]->key <<
                                    "' color R, left is exist, right is NULL\n";
                                return false;
                            }

                        if (buff[now][i]->left != NULL)
                        {
                            if (buff[now][i]->left->color == 'R')
                            {
                                cout << "At '" << buff[now][i]->key <<
                                    "' color R, left is R\n";
                                return false;
                            }
                            else if (buff[now][i]->right == NULL)
                            {
                                cout << "At '" << buff[now][i]->key <<
                                    "' color R, left is B, right is NULL\n";
                                return false;
                            }
                            else if (buff[now][i]->right->color == 'R')
                            {
                                cout << "At '" << buff[now][i]->key <<
                                    "' color R, left is B, right is R\n";
                                return false;
                            }
                            else
                            {
                                buff[far][max_f] = buff[now][i]->left;
                                max_f += 1;
                                buff[far][max_f] = buff[now][i]->right;
                                max_f += 1;
                            }
                        }
                    }
                }

                if (max_f == 0)
                    break;

                swap(now, far);
                swap(max_n, max_f);
            }

            delete[] buff[0];
            delete[] buff[1];

            return true;
        }
    }


private:
    elem* start;
    int number;


    struct answer get_past(elem* ppt)
    {
        answer res;
        res.ppt = ppt->past;

        if (ppt->past == NULL)
            res.direct = 'N';
        else
        {
            if (ppt->past->left == ppt)
                res.direct = 'L';
            else
                res.direct = 'R';
        }

        return res;
    }

    int get_status(struct elem* ppt)
    {
        char rt, lt;

        if (ppt->left != NULL)
        {
            if (ppt->left->color == 'B')
                lt = 'B';
            else
                lt = 'R';
            //cout << "lt = " << lt << endl;
        }
        else
            lt = 'N';

        if (ppt->right != NULL)
        {
            if (ppt->right->color == 'B')
                rt = 'B';
            else
                rt = 'R';
        }
        else
            rt = 'N';

        if (rt == 'R' and lt == 'R')
            return 2;

        if (lt == 'R')
            return -1;

        if (rt == 'R')
        {
            if (ppt->right->right != NULL)
            {
                if (ppt->right->right->color == 'R')
                    return 1;
            }
        }

        return 0;
    }

    void create_wires(elem* par, elem* chil, char direct)
    {
        //cout << "create_wires\n";
        if (direct == 'R')
            par->right = chil;

        if (direct == 'L')
            par->left = chil;

        if (chil != NULL)
            chil->past = par;
    }

    void create_wires(elem* par, elem* chil, char direct, int flag)
    {
        cout << "flag = " << flag;
        if (direct == 'R')
            par->right = chil;

        if (direct == 'L')
            par->left = chil;

        if (chil != NULL)
        {
            chil->past = par;
            cout << "  alright\n";
        }
        else
        {
            cout << "  fale!!!\n";
            exit(1);
        }
    }

    struct elem * create_elem(char color, char* key, float data)
    {
        struct elem* ppt = new elem[1];

        ppt->key = copy_str(key);
        ppt->color = color;
        ppt->data = data;

        ppt->past = NULL;
        ppt->left = NULL;
        ppt->right = NULL;

        return ppt;
    }

    struct answer find(char* key)
    {
        //int counter = 0;
        elem* curr = start;
        elem* buff;
        int res;
        answer ans;

        while (1)
        {
            //counter += 1;
            res = strcmp(curr->key, key);
            buff = NULL;

            if (res < 0)
                buff = curr->right;

            if (res > 0)
                buff = curr->left;

            if (res == 0)
            {
                ans.direct = 'N';
                ans.ppt = curr;

                return ans;
            }

            if (buff == NULL)
            {
                if (res < 0)
                    ans.direct = 'R';
                else
                    ans.direct = 'L';

                ans.ppt = curr;
               // f << "counter = " << counter << endl;
                return ans;
            }
            else
                curr = buff;
        }
    }

    elem* left_ret(elem* curr)
    {
        elem* new_curr = curr->right;
        elem* rt_lt_tail = curr->right->left;

        new_curr->color = curr->color;
        curr->color = 'R';

        create_wires(new_curr, curr, 'L');
        create_wires(curr, rt_lt_tail, 'R');

        return new_curr;
    }

    void swap_color(elem* curr)
    {
        curr->left->color == 'R' ? curr->left->color = 'B' : curr->left->color = 'R';
        curr->right->color == 'R' ? curr->right->color = 'B' : curr->right->color = 'R';
        curr->color == 'R' ? curr->color = 'B' : curr->color = 'R';
    }

    elem* right_ret(elem* curr)
    {
        elem* new_curr = curr->left;
        elem* lt_rt_tail = curr->left->right;

        new_curr->color = curr->color;
        curr->color = 'R';

        create_wires(new_curr, curr, 'R');
        create_wires(curr, lt_rt_tail, 'L');

        return new_curr;
    }

    void del_elem(elem* ppt)
    {
        //cout << "del_elem = " << ppt->key << endl;
        delete[] ppt->key;
        delete[] ppt;
    }

};

int main()
{
    R_B_tree test;
    char* arg,* arg_2;
    int i = 999;
    fstream f;
    f.open("out.txt", ios::out | ios::trunc);
    bool res;
    int size = 1000;
    srand(time(NULL));
    vector <char*> buff;

    for (i = size; i < size * 2; i += 1)
    {
        arg = rand_str(5);
        res = test.add(arg, i * 2);
        buff.push_back(copy_str(arg));
        delete[] arg;
    }

   // test.see_first();
    test.post(f);
    
    for (i = 0; i < size; i += 1)
    {
        int one, two;
        one = rand() % size;
        two = rand() % size;

        swap(buff[one], buff[two]);
    }

    for (i = 2; i < size; i += 1)
    {
        cout << "i = " << i << endl;
        if (test.del(buff[i]) == false)
            cout << "false\n";

        bool ans = test.check_struct();
        if (ans == false)
            break;
        
    }

    
    test.post(f);
    
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
