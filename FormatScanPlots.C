void FormatScanPlot(TGraph *scan){

        scan->GetXaxis()->SetTitleFont(132);
        scan->GetXaxis()->SetLabelFont(132);
        scan->GetYaxis()->SetTitleFont(132);
        scan->GetYaxis()->SetLabelFont(132);
        scan->GetXaxis()->CenterTitle();
        scan->GetXaxis()->SetTitle("Channel");
        scan->GetYaxis()->CenterTitle();
        scan->GetYaxis()->SetTitle("Counts");
        scan->GetXaxis()->SetLabelSize(0.05);
        scan->GetXaxis()->SetTitleSize(0.05);
        scan->GetYaxis()->SetLabelSize(0.05);
        scan->GetYaxis()->SetTitleSize(0.05);
        scan->GetYaxis()->SetTitleOffset(1.01);

}
