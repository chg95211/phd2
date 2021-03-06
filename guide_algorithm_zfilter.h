/*
 *  guide_algorithm_zfilter.h
 *  PHD Guiding
 *
 *  Created by Ken Self
 *  Copyright (c) 2018 Ken Self
 *  All rights reserved.
 *
 *  This source code is distributed under the following "BSD" license
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *    Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *    Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *    Neither the name of openphdguiding.org nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef GUIDE_ALGORITHM_ZFILTER_H_INCLUDED
#define GUIDE_ALGORITHM_ZFILTER_H_INCLUDED

#include "zfilterfactory.h"

class GuideAlgorithmZFilter : public GuideAlgorithm
{
    int    m_filter;
    std::vector<double> m_xv, m_yv;  // Historical values up to m_order
    std::vector<double> m_xcoeff, m_ycoeff;
    int m_order;
    double m_gain;
    double m_minMove;
    double m_sumCorr; // Sum of all corrections issued

    ZFilterFactory *m_pFactory;
    class Filter
    {
    public:
        FILTER_DESIGN design;
        int order;
        double corner;
        Filter(FILTER_DESIGN d, int o, double c) :design(d), order(o), corner(c) {};
        std::string getname() const;
    };
    std::vector<Filter> m_FilterList; // Filter options

protected:
    class GuideAlgorithmZFilterConfigDialogPane : public ConfigDialogPane
    {
        GuideAlgorithmZFilter *m_pGuideAlgorithm;
        wxChoice         *m_pFilter;  // Listbox for filters
        wxSpinCtrlDouble *m_pMinMove;

    public:
        GuideAlgorithmZFilterConfigDialogPane(wxWindow *pParent, GuideAlgorithmZFilter *pGuideAlgorithm);
        virtual ~GuideAlgorithmZFilterConfigDialogPane(void);

        virtual void LoadValues(void);
        virtual void UnloadValues(void);
        virtual void HandleBinningChange(int oldBinVal, int newBinVal);
    };

    class GuideAlgorithmZFilterGraphControlPane : public GraphControlPane
    {
    public:
        GuideAlgorithmZFilterGraphControlPane(wxWindow *pParent, GuideAlgorithmZFilter *pGuideAlgorithm, const wxString& label);
        ~GuideAlgorithmZFilterGraphControlPane(void);

    private:
        GuideAlgorithmZFilter *m_pGuideAlgorithm;
        wxSpinCtrlDouble *m_pMinMove;

        void OnMinMoveSpinCtrlDouble(wxSpinDoubleEvent& evt);
    };

    int GetFilter(void);
    bool SetFilter(int Order);
    virtual double GetMinMove(void) const;
    virtual bool SetMinMove(double minMove);

    friend class GuideAlgorithmZFilterConfigDialogPane;

public:
    GuideAlgorithmZFilter(Mount *pMount, GuideAxis axis);
    virtual ~GuideAlgorithmZFilter(void);
    virtual GUIDE_ALGORITHM Algorithm(void) const;

    virtual void reset(void);
    virtual double result(double input);
    virtual ConfigDialogPane *GetConfigDialogPane(wxWindow *pParent);
    virtual GraphControlPane *GetGraphControlPane(wxWindow *pParent, const wxString& label);
    virtual wxString GetSettingsSummary() const;
    virtual wxString GetGuideAlgorithmClassName(void) const { return "ZFilter"; }
    virtual void GetParamNames(wxArrayString& names) const;
    virtual bool GetParam(const wxString& name, double *val) const;
    virtual bool SetParam(const wxString& name, double val);
};

inline int GuideAlgorithmZFilter::GetFilter(void)
{
    return m_filter;
}

inline double GuideAlgorithmZFilter::GetMinMove(void) const
{
    return m_minMove;
}

inline std::string GuideAlgorithmZFilter::Filter::getname() const
{
    switch (design)
    {
    case BUTTERWORTH: return "Butterworth";
    case BESSEL: return "Bessel";
    case CHEBYCHEV: return "Chebychev";
    default: return "Unknown filter";
    }
}
#endif /* GUIDE_ALGORITHM_ZFILTER_H_INCLUDED */
