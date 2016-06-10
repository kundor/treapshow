#include "TeXout.h"
#include <utility>

template <class Node, class Ptr/*, class Fmt*/>
struct binTreeTeX {
    Ptr head;
    Ptr Node::* left;
    Ptr Node::* right;
    /*Fmt fmt;*/
    void output(TeXout& tex, Ptr n) const {
        tex << "node { \\textbf{" << n->v << "}"
               "\\nodepart{second}{\\small " << n->k << "}} ";
        if ((*n).*left) {
            tex << "child { ";
            output(tex, (*n).*left);
            tex << "}\n";
       /* } else if ((*n).*right) {
            tex << "child {}\n"; // empty child ok?
			// This was to correctly place right children
			// with no left sibling, but it turns out to be
			// nicer without it
			*/
        }
        if ((*n).*right) {
            tex << "child { ";
            output(tex, (*n).*right);
            tex << "}\n";
        }
    }
};


template <class Ptr, typename Node = decltype(*std::declval<Ptr>())/*, class Fmt*/>
binTreeTeX<Node, Ptr/*, Fmt*/> autoTree(Ptr head, Ptr Node::* left, Ptr Node::* right/*, Fmt fmt*/) {
    return {head, left, right/*, fmt*/};
}

template <class Node, class Ptr/*, class Fmt*/>
TeXout& operator<<(TeXout& tex, const binTreeTeX<Node, Ptr>& t) {
	tex.usepackage("tikz");
    tex.usetikzlibrary("shapes");
    tex << "\\begin{tikzpicture}[sibling distance=10em,"
           "every node/.style = {draw, align=center, shape=rectangle split,"
           "rectangle split parts=2, rounded corners}]\n"
           "\\";
    t.output(tex, t.head);
    tex << "\\end{tikzpicture}\n";
    return tex;
}

