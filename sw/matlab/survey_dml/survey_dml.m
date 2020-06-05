%-------------------------------------------------------------------------------
  %
  %  Filename       : survey_dml.m
  %  Author         : Huang Leilei
  %  Created        : 2020-05-20
  %  Description    : survey_dml
  %
%-------------------------------------------------------------------------------
  %
  %  Modified       : 2020-05-21 by HLL
  %  Description    : updated according to survey dbf
  %  Modified       : 2020-05-25 by HLL
  %  Description    : plot optimized
  %  Modified       : 2020-06-04 by HLL
  %  Description    : core part of dml moved to survey_dml_core_?
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
%NUMB_RND         = 1;                               % number of test rounds
 NUMB_RND         = 1000;

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

%DATA_ANG_OBJ     = 0;                               % data   of objects' angles     in degree
%DATA_ANG_OBJ     = [-3, 3];
%DATA_ANG_OBJ     = [-6, 6];
 DATA_ANG_OBJ     = [-12, 12];
%DATA_ANG_OBJ     = [-24, 24];
%DATA_ANG_OBJ     = [0, 12];

 DATA_POW_OBJ     = [0, 0];                          % data   of objects' powers     in dB
 DATA_SNR         = 20;                              % data   of snr                 in dB

%DATA_DLT_ANG_TST = 0.1;                             % data   of test angles' delta  in degree
 DATA_DLT_ANG_TST = 1;

 DATA_RNG_ANG_TST = 89;                              % data   of test angles' range  in degree




%%*** DERIVED PARAMETER *********************************************************
DATA_ANG_TST = -DATA_RNG_ANG_TST:DATA_DLT_ANG_TST:DATA_RNG_ANG_TST;
NUMB_ANT     = numel(DATA_DIS_ANT);
NUMB_OBJ     = numel(DATA_ANG_OBJ);
NUMB_ANG_TST = numel(DATA_ANG_TST);




%*** MAIN BODY *****************************************************************
%% core
idxAngRnd = survey_dml_core_1(NUMB_RND, NUMB_SMP, INDX_FIG, DATA_DIS_ANT, DATA_COE_WIN, DATA_ANG_OBJ, DATA_POW_OBJ, DATA_SNR, DATA_DLT_ANG_TST, DATA_RNG_ANG_TST);;




%% analyze
if NUMB_RND > 1
    figure(INDX_FIG); INDX_FIG = INDX_FIG + 1;
    for idxObj = 1:2
        % calculate
        datAng = DATA_ANG_TST(idxAngRnd(idxObj, :));
        err = sum((datAng - DATA_ANG_OBJ(min(NUMB_OBJ, idxObj))) .^ 2) / NUMB_RND;
        % log
        fprintf('most solved angle (and averaged SSE) of object %d is %.2f (and %.2f), which should be %.2f\n', idxObj, mode(datAng), err, DATA_ANG_OBJ(min(NUMB_OBJ, idxObj)));
        % plot
        subplot(2, 1, idxObj);
        histogram(datAng, DATA_ANG_TST - 0.5 * DATA_DLT_ANG_TST);
        % tune figure
        set(gcf, 'position', [800, 300, 1200, 400]);
        grid on;
        title(['histogram of solved angle (object ', num2str(idxObj)', ')']);
        xlabel('angle (degree)');
        ylabel('number');
        % save figure
        fig = getframe(gcf);
        img = frame2im(fig);
        imwrite(img, 'dump/histo.png');
    end
end




%*** POST **********************************************************************
toc;
