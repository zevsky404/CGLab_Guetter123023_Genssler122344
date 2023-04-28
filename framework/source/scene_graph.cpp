#include <string>
#include "scene_graph.hpp"

//
// Created by neo on 27.04.23.
//
const std::__cxx11::basic_string<char> &SceneGraph::getName() const {
    return name_;
}

void SceneGraph::setName(const std::__cxx11::basic_string<char> &name) {
    name_ = name;
}

const Node &SceneGraph::getRoot() const {
    return root_;
}

void SceneGraph::setRoot(const Node &root) {
    root_ = root;
}

std::ostream &operator<<(std::ostream &os, const SceneGraph &graph) {
    os << "name_: " << graph.name_ << " root_: " << graph.root_;
    return os;
}

/*void Node::printGraph(bool show_transformations) {
    std::cout << *this << std::endl;
    if (show_transformations) {
        std::cout << local_transform_
    }
}*/
