#ifndef WAVEFORMRENDERERSIGNALBASE_H
#define WAVEFORMRENDERERSIGNALBASE_H

#include "waveformrendererabstract.h"
#include "waveformsignalcolors.h"
#include "skin/skincontext.h"

class ControlObject;
class ControlObjectThread;

class WaveformRendererSignalBase : public WaveformRendererAbstract {
public:
    explicit WaveformRendererSignalBase(WaveformWidgetRenderer* waveformWidgetRenderer);
    virtual ~WaveformRendererSignalBase();

    virtual bool init();
    virtual void setup(const QDomNode& node, const SkinContext& context);

    virtual bool onInit() {return true;}
    virtual void onSetup(const QDomNode &node) = 0;

protected:
    void deleteControls();

protected:
    ControlObjectThread* m_pLowFilterControlObject;
    ControlObjectThread* m_pMidFilterControlObject;
    ControlObjectThread* m_pHighFilterControlObject;

    ControlObjectThread* m_pLowKillControlObject;
    ControlObjectThread* m_pMidKillControlObject;
    ControlObjectThread* m_pHighKillControlObject;

    const WaveformSignalColors* m_pColors;
    qreal m_axesColor_r, m_axesColor_g, m_axesColor_b, m_axesColor_a;
    qreal m_signalColor_r, m_signalColor_g, m_signalColor_b;
    qreal m_lowColor_r, m_lowColor_g, m_lowColor_b;
    qreal m_midColor_r, m_midColor_g, m_midColor_b;
    qreal m_highColor_r, m_highColor_g, m_highColor_b;

    Qt::Alignment m_alignment;
};

#endif // WAVEFORMRENDERERSIGNALBASE_H
