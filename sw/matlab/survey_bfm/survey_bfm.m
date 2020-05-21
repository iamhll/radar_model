%-------------------------------------------------------------------------------
  %
  %  Filename       : survey_bfm.m
  %  Author         : Huang Leilei
  %  Created        : 2020-05-21
  %  Description    : survey bfm
  %
%-------------------------------------------------------------------------------

%*** INIT **********************************************************************
clc;
clear;
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
%% prepare datCoe
datCoeTst = ones(NUMB_ANT, NUMB_ANG_TST);
for idxAng = 1:NUMB_ANG_TST
    datAng = DATA_ANG_TST(idxAng);
    datCoe = exp(-1i * 2 * pi * DATA_DIS_ANT * sin(datAng / 180 * pi));
    datCoeTst(:, idxAng) = datCoe;
end


%% traverse
idxAngRnd = ones(2, NUMB_RND);
for idxRnd = 1:NUMB_RND
    %% prepare datSig
    datSig = 0;
    for idxObj = 1:NUMB_OBJ
        datAmp = 10^(DATA_POW_OBJ(idxObj) / 20) / 2^0.5 * DATA_COE_WIN;
        datPha = exp(-1i * 2 * pi * rand);
        %datPha = exp(-1i * 2 * pi * 0);
        datSig = datSig + datAmp .* exp(1i * 2 * pi * DATA_DIS_ANT * sin(DATA_ANG_OBJ(idxObj) / 180 * pi)) * datPha;
    end


    %% prepare datSmp
    datSmpSum = 0;
    for idxSmp = 1:NUMB_SMP
        datSmpTmp = awgn(datSig, DATA_SNR, 'measured');
        datSmpSum = datSmpSum + datSmpTmp;
    end
    datSmp = datSmpSum / NUMB_SMP;


    %% get best cost and index
    datPowTst = ones(1, NUMB_ANG_TST);
    for idxAng = 1:NUMB_ANG_TST
        datPowTmp = datCoeTst(:, idxAng) .* datSmpSum;
        datPowTst(idxAng) = 20*log10(abs(sum(datPowTmp)));
    end
    [datPks, idxPks] = findpeaks(datPowTst);
    [datSrt, idxSrt] = sort(datPks, 'descend');
    idxAng = idxPks(idxSrt);
    if numel(idxAng) >= 2 && datSrt(1) - datSrt(2) <= 3
        if idxAng(1) < idxAng(2)
            idxAng = [idxAng(1), idxAng(2)];
        else
            idxAng = [idxAng(2), idxAng(1)];
        end
    elseif numel(idxAng) >= 2
        idxAng = idxAng(1);
    else
        idxAng = round(NUMB_ANG_TST / 2);
    end
    idxAngRnd(:, idxRnd) = idxAng;


    %% plot
    if NUMB_RND == 1
        % open figure
        figure(INDX_FIG); INDX_FIG = INDX_FIG + 1;
        % curve
        plot(DATA_ANG_TST, datPowTst);
        hold on;
        % peak
        for idxObj = 1:numel(idxAng)
            datAng = DATA_ANG_TST(idxAng(idxObj));
            datPow = datPowTst(idxAng(idxObj));
            plot(datAng, datPow, 'xk', 'MarkerSize', 10);
            text(datAng - 0.5, datPow + 1.5,                                            ...
                ['\downarrow', num2str(datAng,'%.2f'), ', ', num2str(datPow,'%.2f')]    ...
            );
        end
        % -3 db point
        plot(DATA_ANG_TST, ones(1, NUMB_ANG_TST) * (max(datPowTst) - 3), '--');
        for idxAng = 2:NUMB_ANG_TST
            if (datPowTst(idxAng - 1) - (max(datPowTst) - 3)) * (datPowTst(idxAng) - (max(datPowTst) - 3)) < 0
                if datPowTst(idxAng - 1) > (max(datPowTst) - 3)
                    datAng = DATA_ANG_TST(idxAng - 1);
                    datPow = datPowTst (idxAng - 1);
                else
                    datAng = DATA_ANG_TST(idxAng);
                    datPow = datPowTst (idxAng);
                end
                plot(datAng, datPow, 'xk', 'MarkerSize', 10);
                text(datAng - 0.5, datPow + 1.5,                                            ...
                    ['\downarrow', num2str(datAng,'%.2f'), ', ', num2str(datPow,'%.2f')]    ...
                );
            end
        end
        hold off;
        % tune figure
        set(gcf, 'position', [1320, 500, 800, 400]);
        grid on;
        axis([min(DATA_ANG_TST), max(DATA_ANG_TST), max(datPowTst) - 20, max(datPowTst) + 5])
        title('bfm curve');
        xlabel('angle (degree)');
        ylabel('power (dB)');
        % save figure
        fig = getframe(gcf);
        img = frame2im(fig);
        imwrite(img, 'dump/curve.png');
    end
end


%% analyze
if NUMB_RND > 1
    figure(INDX_FIG); INDX_FIG = INDX_FIG + 1;
    for idxObj = 1:NUMB_OBJ
        % calculate
        datAng = DATA_ANG_TST(idxAngRnd(idxObj, :));
        err = sum((datAng - DATA_ANG_OBJ(idxObj)) .^ 2) / NUMB_RND;
        % log
        fprintf('most solved angle (and averaged SSE) of object %d is %.2f (and %.2f), which should be %.2f\n', idxObj, mode(datAng), err, DATA_ANG_OBJ(idxObj));
        % plot
        subplot(NUMB_OBJ, 1, idxObj);
        histogram(datAng, DATA_ANG_TST - 0.5 * DATA_DLT_ANG_TST);
        % tune figure
        set(gcf, 'position', [1320, 500, 1200, 400]);
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
