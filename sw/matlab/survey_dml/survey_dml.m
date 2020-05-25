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
DATA_ANG_TST     = -DATA_RNG_ANG_TST:DATA_DLT_ANG_TST:DATA_RNG_ANG_TST;
NUMB_ANT         = numel(DATA_DIS_ANT);
NUMB_OBJ         = numel(DATA_ANG_OBJ);
NUMB_ANG_TST     = numel(DATA_ANG_TST);
DATA_ANG_AMB_MAX = asin(1 / (DATA_DIS_ANT(2  ) - DATA_DIS_ANT(1)) / 2) / pi * 180;
fprintf('max non-ambiguity angle of objects is about %.2f degree\n', DATA_ANG_AMB_MAX);




%*** MAIN BODY *****************************************************************
%% prepare datP
datPTst = ones(NUMB_ANT, NUMB_ANT, NUMB_ANG_TST, NUMB_ANG_TST);
for idxAng0 = 1:NUMB_ANG_TST
    datAng0 = DATA_ANG_TST(idxAng0);
    for idxAng1 = (idxAng0+1):NUMB_ANG_TST
        datAng1 = DATA_ANG_TST(idxAng1);
        datA = [exp(1i * 2 * pi * DATA_DIS_ANT * sin(datAng0 / 180 * pi))     ...
               ,exp(1i * 2 * pi * DATA_DIS_ANT * sin(datAng1 / 180 * pi))]    ...
        ;
        datP = datA * (datA' * datA)^-1 * datA';
        datPTst(:, :, idxAng0, idxAng1) = datP;
    end
end


%% traverse
idxAngRnd = ones(2, NUMB_RND);
for idxRnd = 1:NUMB_RND
    %% prepare datSig
    datSig = 0;
    for idxObj = 1:NUMB_OBJ
        datAmp = 10^(DATA_POW_OBJ(idxObj) / 20) / 2^0.5;
        datPha = exp(-1i * 2 * pi * rand);
        datSig = datSig + datAmp .* exp(1i * 2 * pi * DATA_DIS_ANT * sin(DATA_ANG_OBJ(idxObj) / 180 * pi)) * datPha;
    end


    %% prepare datR
    datRSum = 0;
    for idxSmp = 1:NUMB_SMP
        datRTmp = awgn(datSig, DATA_SNR, 'measured');
        datRTmp = datRTmp .* DATA_COE_WIN;
        datRSum = datRSum + datRTmp * datRTmp';
    end
    datR = datRSum / NUMB_SMP;
    %datSmpSum = 0;
    %for idxSmp = 1:NUMB_SMP
    %    datSmpTmp = awgn(datSig, DATA_SNR, 'measured');
    %    datSmpTmp = datSmpTmp .* DATA_COE_WIN;
    %    datSmpSum = datSmpSum + datSmpTmp;
    %end
    %datSmp = datSmpSum / NUMB_SMP;
    %datR = datSmp * datSmp';

    %% get best cost and index
    datPowTst = ones(NUMB_ANG_TST, NUMB_ANG_TST) * -inf;
    datPowBst = -inf;
    for idxAng0 = 1:NUMB_ANG_TST
        for idxAng1 = idxAng0+1:NUMB_ANG_TST
            datPowTmp = datPTst(:, :, idxAng0, idxAng1) * datR;
            datPow = 20 * log10(abs(trace(datPowTmp)));
            %datPowTmpA = datPTst(:, :, idxAng0, idxAng1) * datRTmp;
            %datPowA = 20 * log10(norm(datPowTmpA));
            if datPow > datPowBst
                datPowBst = datPow;
                idxAngRnd(:, idxRnd) = [idxAng0, idxAng1];
            end
            datPowTst(idxAng0, idxAng1) = datPow;
        end
    end


    %% plot
    if NUMB_RND == 1
        % open figure
        figure(INDX_FIG); INDX_FIG = INDX_FIG + 1;
        % surface
        surface(DATA_ANG_TST, DATA_ANG_TST, datPowTst - max(max(datPowTst)), 'LineStyle', 'none');
        hold on;
        % peak
        datAng0 = DATA_ANG_TST(idxAngRnd(1, idxRnd));
        datAng1 = DATA_ANG_TST(idxAngRnd(2, idxRnd));
        plot(DATA_ANG_TST, ones(1, NUMB_ANG_TST) * datAng0, '--');
        plot(ones(1, NUMB_ANG_TST) * datAng1, DATA_ANG_TST, '--');
        text(datAng1 + 3, datAng0 + 5, [num2str(datAng0,'%.1f'), ', ', num2str(datAng1,'%.1f')]);
        hold off;
        % tune figure
        set(gcf, 'position', [800, 300, 500, 400]);
        grid on;
        title('dml surface');
        xlabel('angle (degree)');
        ylabel('angle (degree)');
        % save figure
        fig = getframe(gcf);
        img = frame2im(fig);
        imwrite(img, 'dump/surface.png');
    end
end


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




%% post
toc;
