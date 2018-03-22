#include <gtest/gtest.h>

#include "../src/core/digraph.h"

TEST(GraphConstruction, Zachary){
    uint32_t order = 34;
    uint32_t size = 78;
    double init_state = 1.0;
    uint32_t batch_size = 1;

    Digraph g = Digraph(order,init_state,batch_size);

    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 3);
    g.add_edge(1, 4);
    g.add_edge(2, 4);
    g.add_edge(3, 4);
    g.add_edge(1, 5);
    g.add_edge(1, 6);
    g.add_edge(1, 7);
    g.add_edge(5, 7);
    g.add_edge(6, 7);
    g.add_edge(1, 8);
    g.add_edge(2, 8);
    g.add_edge(3, 8);
    g.add_edge(4, 8);
    g.add_edge(1, 9);
    g.add_edge(3, 9);
    g.add_edge(3, 10);
    g.add_edge(1, 11);
    g.add_edge(5, 11);
    g.add_edge(6, 11);
    g.add_edge(1, 12);
    g.add_edge(1, 13);
    g.add_edge(4, 13);
    g.add_edge(1, 14);
    g.add_edge(2, 14);
    g.add_edge(3, 14);
    g.add_edge(4, 14);
    g.add_edge(6, 17);
    g.add_edge(7, 17);
    g.add_edge(1, 18);
    g.add_edge(2, 18);
    g.add_edge(1, 20);
    g.add_edge(2, 20);
    g.add_edge(1, 22);
    g.add_edge(2, 22);
    g.add_edge(24, 26);
    g.add_edge(25, 26);
    g.add_edge(3, 28);
    g.add_edge(24, 28);
    g.add_edge(25, 28);
    g.add_edge(3, 29);
    g.add_edge(24, 30);
    g.add_edge(27, 30);
    g.add_edge(2, 31);
    g.add_edge(9, 31);
    g.add_edge(1, 32);
    g.add_edge(25, 32);
    g.add_edge(26, 32);
    g.add_edge(29, 32);
    g.add_edge(3, 33);
    g.add_edge(9, 33);
    g.add_edge(15, 33);
    g.add_edge(16, 33);
    g.add_edge(19, 33);
    g.add_edge(21, 33);
    g.add_edge(23, 33);
    g.add_edge(24, 33);
    g.add_edge(30, 33);
    g.add_edge(31, 33);
    g.add_edge(32, 33);
    g.add_edge(9, 34);
    g.add_edge(10, 34);
    g.add_edge(14, 34);
    g.add_edge(15, 34);
    g.add_edge(16, 34);
    g.add_edge(19, 34);
    g.add_edge(20, 34);
    g.add_edge(21, 34);
    g.add_edge(23, 34);
    g.add_edge(24, 34);
    g.add_edge(27, 34);
    g.add_edge(28, 34);
    g.add_edge(29, 34);
    g.add_edge(30, 34);
    g.add_edge(31, 34);
    g.add_edge(32, 34);
    g.add_edge(33, 34);

    ASSERT_EQ(g.get_size(), size);
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


