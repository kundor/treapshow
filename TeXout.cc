#define _GLIBCXX_USE_C99 1
#include <string>
#include "TeXout.h"
#include <cmath> // lround
#include <ostream>

using std::to_string;

void TeXout::documentclass(std::string docuclass) {
    // Potentially validate against approved document classes
    doc_class = docuclass;
}

void TeXout::classopt(std::string opt) {
    classopts.insert(opt);
}

void TeXout::usepackage(std::string package) {
    // potentially worry about package ordering and dependencies
    packages.insert(package);
}

void TeXout::usetikzlibrary(std::string lib) {
    tikzlibraries.insert(lib);
}


void TeXout::addtopreamble(std::string pre) {
    preamble += pre;
}

TeXout& TeXout::operator<<(std::string s) {
    doc += s;
    return *this;
}

TeXout& TeXout::operator<<(char c) {
    doc += c;
    return *this;
}

TeXout& TeXout::operator<<(double d) {
    long i = std::lround(d*10);
    if (i < 0) {
        doc += '-';
        i *= -1;
    }
    doc += std::to_string(i/10) + '.' + std::to_string(i%10);
    // do fixed, setprecision(1) without stringstreams
    return *this;
}

TeXout& TeXout::operator<<(int i) {
    doc += std::to_string(i);
    return *this;
}

TeXout& TeXout::operator<<(unsigned u) {
    doc += std::to_string(u);
    return *this;
}

TeXout& TeXout::operator<<(long unsigned u) {
    doc += std::to_string(u);
    return *this;
}

std::ostream& TeXout::tostream(std::ostream& s) {
    if (doc_class == "standalone") {
        auto ft = packages.find("tikz");
        if (ft != packages.end()) {
            packages.erase(ft);
            classopts.insert("tikz");
        }
    }
    s << "\\documentclass";
    if (!classopts.empty()) {
        s << '[';
        for (auto sit = classopts.begin();;) {
            s << *sit;
            if (++sit == classopts.end()) break;
            s << ',';
        }
        s << ']';
    }
    s << '{' << doc_class << "}\n";
    for (auto sit = packages.begin(); sit != packages.end(); ++sit) {
        if (doc_class == "amsart" && *sit == "amsmath")
            continue;
        s << "\\usepackage{" << *sit << "}\n";
    }
    if (!tikzlibraries.empty()) {
        s << "\\usetikzlibrary{";
        for (auto sit = tikzlibraries.begin();;) {
            s << *sit;
            if (++sit == tikzlibraries.end()) break;
            s << ',';
        }
        s << "}\n";
    }
    return s << preamble
             << "\\begin{document}\n"
             << doc
             << "\\end{document}\n";
}

