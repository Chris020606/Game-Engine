//
// Created by Christian Cruz on 12/22/25.
//

#ifndef LAYERSTACK_H
#define LAYERSTACK_H
#include <vector>

namespace Nebula {
    class Layer;

class LayerStack {


    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer *layer);
        void PushOverlay(Layer *layer);
        void PopLayer(Layer *layer);
        void PopOverlay(Layer *layer);

        std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_layers.end(); }
    private:
        std::vector<Layer*> m_layers;
        unsigned int m_layerIndex = 0;
    };
} // Nebula

#endif //LAYERSTACK_H
