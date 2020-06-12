%-------------------------------------------------------------------------------
  %
  %  Filename       : survey_dml.m
  %  Author         : Huang Leilei
  %  Created        : 2020-06-12
  %  Description    : survey_dml
  %                   (based on SW/M/survey_dml/survey_dml.m)
  %
%-------------------------------------------------------------------------------

%*** INIT **********************************************************************
%clc;
%clear;
close all;
if ~exist('dump', 'dir')
    mkdir dump
end
tic;




%*** PARAMETER *****************************************************************
 FLAG_FIG         = 0;                               % flag of inner figure

 NUMB_RND         = 1;                               % number of test rounds
%NUMB_RND         = 1000;

 NUMB_SMP         = 1;                               % number of samples
 INDX_FIG         = 1;                               % index  of figure

 DATA_DIS_ANT     = 0.5 * (0:3);                     % data   of antenna's distances in lamda
%DATA_DIS_ANT     = 0.5 * [0,1,4,6];
%DATA_DIS_ANT     = 0.5 * [(0:3),(0:3)+3+1];
%DATA_DIS_ANT     = 0.5 * [[0,1,4,6],[0,1,4,6]+6+1];
 DATA_DIS_ANT     = DATA_DIS_ANT';

 DATA_COE_WIN     = ones(1, numel(DATA_DIS_ANT));    % data   of window coefficient  in "1"
%DATA_COE_WIN     = [0.5, 1, 1 ,0.5];
 DATA_COE_WIN     = DATA_COE_WIN';

 DATA_POW_OBJ     = [0, 0];                          % data   of objects' powers     in dB
 DATA_SNR         = 20;                              % data   of snr                 in dB

%DATA_DLT_ANG_TST = 0.1;                             % data   of test angles' delta  in degree
 DATA_DLT_ANG_TST = 1;

 DATA_RNG_ANG_TST = 89;                              % data   of test angles' range  in degree




%%*** DERIVED PARAMETER *********************************************************
DATA_ANG_TST     = -DATA_RNG_ANG_TST:DATA_DLT_ANG_TST:DATA_RNG_ANG_TST;
NUMB_ANT         = numel(DATA_DIS_ANT);
NUMB_ANG_TST     = numel(DATA_ANG_TST);
DATA_ANG_AMB_MAX = asin(1 / (DATA_DIS_ANT(2  ) - DATA_DIS_ANT(1)) / 2) / pi * 180;
fprintf('max non-ambiguity angle of objects is about %.2f degree\n', DATA_ANG_AMB_MAX);




%*** MAIN BODY *****************************************************************
%% core
DATA_ANG_OBJ_LST = 1:30;
SIZE_ANG_OBJ_LST = numel(DATA_ANG_OBJ_LST);
idxAng    = zeros(2, SIZE_ANG_OBJ_LST);
datPowDlt = zeros(1, SIZE_ANG_OBJ_LST);
for idx = DATA_ANG_OBJ_LST
    %DATA_ANG_OBJ = idx;
    DATA_ANG_OBJ = [-idx, idx];
    fprintf('testing angle %02d ...\n', idx);
    [idxAng(:,idx), datPowDlt(idx)] = survey_dml_core(FLAG_FIG, NUMB_RND, NUMB_SMP, INDX_FIG, DATA_DIS_ANT, DATA_COE_WIN, DATA_ANG_OBJ, DATA_POW_OBJ, DATA_SNR, DATA_DLT_ANG_TST, DATA_RNG_ANG_TST);
end


%% analyze
if ~FLAG_FIG
    % plot
    figure(INDX_FIG); INDX_FIG = INDX_FIG + 1;
    plot(DATA_ANG_OBJ_LST, datPowDlt);
    legend('dbf\_max - dml');
    % tune figure
    set(gcf, 'position', [800, 300, 1200, 400]);
    axis([1,30,-4,1]);
    grid on;
    title('delta power vs object angle');
    xlabel('angle (degree)');
    ylabel('power (dB)');
    % save figure
    fig = getframe(gcf);
    img = frame2im(fig);
    imwrite(img, 'dump/power_vs_angle.png');
end




%*** POST **********************************************************************
toc;
