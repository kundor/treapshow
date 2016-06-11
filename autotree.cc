#include "TeXout.h"
#include <utility>

template <class Node, class Ptr/*, class Fmt*/>
struct binTreeTeX {
    Ptr head;
    Ptr Node::* left;
    Ptr Node::* right;
    const std::string& msg;
    /*Fmt fmt;*/
    void output(TeXout& tex, Ptr n, bool topnode = false) const {
        if (topnode) tex << "\\node(root)";
        else tex << "node";
        tex << " { \\textbf{" << n->v << "}"
               "\\nodepart{second}{\\small " << n->k << "}} ";
        if ((*n).*left) {
            tex << "\nchild { ";
            output(tex, (*n).*left);
            tex << '}';
        } else if ((*n).*right) {
            tex << "\nchild[missing] {}";
			// This is to correctly place right children with no sibling
        }
        if ((*n).*right) {
            tex << "\nchild { ";
            output(tex, (*n).*right);
            tex << '}';
        } else if ((*n).*left) {
            tex << "\nchild[missing] {}";
			// This is to correctly place left children with no sibling
        }
        tex << (topnode ? ";\n" : " ");
    }
};


template <class Ptr, typename Node = decltype(*std::declval<Ptr>())/*, class Fmt*/>
binTreeTeX<Node, Ptr/*, Fmt*/> autoTree(Ptr head, Ptr Node::* left, Ptr Node::* right, const std::string& msg = ""/*, Fmt fmt*/) {
    return {head, left, right, msg};
}

template <class Node, class Ptr/*, class Fmt*/>
TeXout& operator<<(TeXout& tex, const binTreeTeX<Node, Ptr>& t) {
	tex.usepackage("tikz");
    tex.usetikzlibrary("shapes");
    tex << "\\begin{tikzpicture}[level 1/.style={sibling distance=12em},"
           "level 2/.style={sibling distance=7em},"
           "level 3/.style={sibling distance=4em},"
           "level 4/.style={sibling distance=2em},"
           "every node/.style = {draw, align=center, shape=rectangle split,"
           "rectangle split parts=2, rounded corners}]\n";
    t.output(tex, t.head, true);
    if (!t.msg.empty())
        tex << "\\node[shape=rectangle,draw=none,above of=root] {" << t.msg << "};\n";
    tex << "\\end{tikzpicture}\n";
    return tex;
}

