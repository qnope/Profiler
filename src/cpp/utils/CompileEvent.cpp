#include "CompileEvent.h"

#include <ltl/algos.h>
#include <ltl/operator.h>
#include <ltl/functional.h>
#include <ltl/Range/actions.h>

using namespace std::chrono;

using namespace ltl;
using namespace ltl::actions;

static bool eventFitInNode(const CompileEvent &event, const CompileEventNode &node) {
    auto beginNode = node.begin;
    auto endNode = node.begin + node.duration;

    auto beginEvent = event.begin;
    auto endEvent = event.begin + event.duration;

    return (beginNode <= beginEvent) && (endEvent <= endNode);
}

uint64_t CompileEventNode::end() const noexcept { return (begin + duration).count(); }
uint64_t CompileEventNode::getBegin() const noexcept { return begin.count(); }

CompileEventNode CompileEventNode::addEvent(CompileEvent event) && {
    assert(eventFitInNode(event, *this));

    if (auto ptr = find_if_ptr(nodeEvents, curry(&eventFitInNode, event))) {
        *ptr = std::move(*ptr).addEvent(std::move(event));
    } else {
        nodeEvents.push_back(CompileEventNode{
            std::move(event.name),          //
            std::move(event.details),       //
            event.begin,                    //
            event.duration,                 //
            std::vector<CompileEventNode>{} //
        });
    }
    return std::move(*this);
}

QVariantList CompileEventNode::getEvents() const noexcept {
    auto to_variant = [](const auto &x) { return QVariant::fromValue(x); };
    auto variantList = nodeEvents | map(to_variant);
    return {variantList.begin(), variantList.end()};
}

CompileEventNode constructTreeEvent(std::vector<CompileEvent> events) {
    auto begin = ltl::min_element_value(events | map(&CompileEvent::begin));
    auto end = ltl::max_element_value(events | map([](auto event) { //
                                          return event.begin + event.duration;
                                      }));
    CompileEventNode node{QObject::tr("Full trace"), "", begin.value_or(0us), end.value_or(0us), {}};

    return accumulate(make_move_range(events), std::move(node), &CompileEventNode::addEvent);
}
