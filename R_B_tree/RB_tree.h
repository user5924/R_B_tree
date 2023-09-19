template <typename TN>
class R_B_tree
{
public:
    struct elem
    {
        char color;

        char* key;
        TN data;

        elem* past;
        elem* left;
        elem* right;
    };

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

    bool add(char* key, TN data)
    {
        if (start == NULL)
        {
            start = create_elem('B', key, data);
        }
        else
        {
            answer ans = find(key);

            if (ans.direct == 'N')
                return false;

            struct elem* buff = create_elem('R', key, data);
            create_wires(ans.ppt, buff, ans.direct);
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
                    curr = right_ret(curr);
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

            if (to_del->color == 'R')
            {
                ans = get_past(to_del);
                if (ans.direct == 'L')
                    ans.ppt->left = NULL;
                else
                    ans.ppt->right = NULL;

                del_elem(to_del);
            }
            else
            {
                elem* tail = NULL;
                ans = get_past(to_del);
                del_elem(to_del);

                while (1)
                {
                    if (ans.ppt == NULL)
                    {
                        start = tail;
                        break;
                    }
                    else
                    {
                        answer past = get_past(ans.ppt);
                        create_wires(ans.ppt, tail, ans.direct);

                        if (ans.direct == 'L')
                        {
                            if (ans.ppt->color == 'B')
                            {
                                if (ans.ppt->right->color == 'R')
                                {
                                    elem* Z1 = ans.ppt->right->left;
                                    elem* curr;
                                    ans.ppt->right->right;
                                    if (Z1->right == NULL or (Z1->right->color == 'B'))
                                    {
                                        curr = left_ret(ans.ppt);
                                        curr->left->color = 'B';
                                        curr->left->right->color = 'R';
                                    }
                                    else
                                    {
                                        elem* X, * Y, * Z1L, * Z1R, * Z2;

                                        X = ans.ppt;
                                        X->color = 'B';
                                        Y = ans.ppt->right;
                                        Y->color = 'R';
                                        Z1 = Y->left;
                                        Z1->color = 'B';
                                        Z1L = Z1->left;
                                        Z1R = Z1->right;
                                        if (Z1R != NULL)
                                            Z1R->color = 'B';
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
                                    create_result_wires(past.ppt, curr, past.direct);

                                    break;
                                }
                                else
                                {
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

                                        create_result_wires(past.ppt, curr, past.direct);
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                if (ans.ppt->right->right == NULL or ans.ppt->right->right->color == 'B')
                                {
                                    ans.ppt->color = 'B';
                                    ans.ppt->right->color = 'R';
                                }
                                else
                                {
                                    elem* curr = left_ret(ans.ppt);
                                    curr->left->color = 'B';
                                    curr->right->color = 'B';

                                    create_result_wires(past.ppt, curr, past.direct);
                                }

                                break;
                            }
                        }
                        else
                        {
                            if (ans.ppt->color == 'R')
                            {
                                elem* curr;
                                if (ans.ppt->left->right == NULL or
                                    ans.ppt->left->right->color == 'B')
                                {
                                    curr = right_ret(ans.ppt);
                                    curr->color = 'B';
                                }
                                else
                                    curr = create_struct(ans.ppt, tail, 'R');

                                create_result_wires(past.ppt, curr, past.direct);
                                break;
                            }
                            else
                            {
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
                                    curr = create_struct(ans.ppt, tail, 'B');
                                    create_result_wires(past.ppt, curr, past.direct);
                                    break;
                                }
                            }
                        }
                    }
                }

            }
        }

        number -= 1;
        if (start != NULL)
            start->past = NULL;
        return true;
    }

    void  post_recurs(fstream& f)
    {
        if (start == NULL)
        {
            f << "No data!\n";
            return;
        }
        
        post_recurs_procedure(f, start);
    }

    void post(fstream& f)
    {
        elem** stack;
        stack = new elem * [number];
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
                f << curr->key << "    " << curr->data << "    " << endl;

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

    TN get_value(char* key)
    {
        answer ans = find(char * key);
        if (ans.direct == 'N')
            return ans.ppt->data;
        else
            return NULL;
    }

    void clear()
    {
        if (start != NULL)
        {
            clear_procedure(start);
            start = NULL;
        }
    }

private:
    elem* start;
    int number;

    void clear_procedure(elem * curr)
    {
        if (curr->left != NULL)
            clear_procedure(curr->left);

        if (curr->right != NULL)
            clear_procedure(curr->right);

        del_elem(curr);
    }
    void  post_recurs_procedure(fstream& f, elem* curr)
    {
        if (curr->left != NULL)
            post_recurs_procedure(f, curr->left);

        f << curr->key << "    " << curr->data << "    " << endl;

        if (curr->right != NULL)
            post_recurs_procedure(f, curr->right);
    }

    void create_result_wires(elem* past, elem* curr, char color)
    {
        if (past != NULL)
            create_wires(past, curr, color);
        else
            start = curr;
    }

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
        if (direct == 'R')
            par->right = chil;

        if (direct == 'L')
            par->left = chil;

        if (chil != NULL)
            chil->past = par;
    }

    struct  elem* create_struct(elem* curr, elem* tail, char color)
    {
        elem* Y, * X, * Z1, * Z2, * Z2L, * Z2R;
        Y = curr;
        Y->color = 'B';
        X = Y->left;
        X->color = 'B';
        Z1 = X->left;
        Z2 = X->right;
        Z2->color = color;
        Z2L = Z2->left;
        Z2R = Z2->right;

        create_wires(X, Z1, 'L');
        create_wires(X, Z2L, 'R');
        create_wires(Y, Z2R, 'L');
        create_wires(Y, tail, 'R');
        create_wires(Z2, X, 'L');
        create_wires(Z2, Y, 'R');

        return Z2;
    }

    struct elem* create_elem(char color, char* key, TN data)
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
        elem* curr = start;
        elem* buff;
        int res;
        answer ans;

        while (1)
        {
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
        delete[] ppt->key;
        delete[] ppt;
    }
};

