//
// Created by Christian Cruz on 12/22/25.
//

#include "LayerStack.h"

namespace Nebula {

    LayerStack::LayerStack() {

    };
    LayerStack::~LayerStack() {
        for (Layer *layer : m_layers) {
            delete &layer;
        }
    }

    void LayerStack::PushLayer(Layer *layer) {
        m_layers.emplace(m_layers.begin() + m_layerIndex, layer);
        m_layerIndex++;
    }

    void LayerStack::PushOverlay(Layer *layer) {
        m_layers.emplace_back(layer);
    }
    void LayerStack::PopLayer(Layer *layer) {
        auto it = std::find(m_layers.begin(), m_layers.end(), layer);
        if (it != m_layers.end()) {
            m_layers.erase(it);
            m_layerIndex--;
        }
    }
    void LayerStack::PopOverlay(Layer *layer) {
        auto it = std::find(m_layers.begin(), m_layers.end(), layer);
        if (it != m_layers.end()) {
            m_layers.erase(it);
        }
    }

} // Nebula