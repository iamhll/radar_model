%-------------------------------------------------------------------------------
  %
  %  Filename       : survey_mainLobeWidth_and_sideLobePower_vs_txDistance.m
  %  Author         : Huang Leilei
  %  Created        : 2020-05-28
  %  Description    : survey dbf
  %
%-------------------------------------------------------------------------------
  %
  %  Modified       : 2020-07-27 by HLL
  %  Description    : maintained
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
 INDX_FIG                = 1;                  % index of figure

 DATA_DIS_ANT_RX         = 0.5 * (0:3);        % data of the distances of rx antenna in lamda
%DATA_DIS_ANT_RX         = 0.5 * [0,1,4,6];
%DATA_DIS_ANT_RX         = 1.0 * (0:3);

 NUMB_ANT_TX             = 2;                  % number                             of the tx antenna
 DATA_DIS_ANT_TX_TST_MIN = 0;                  % minimum data of the test distances of the tx antenna in lamda
 DATA_DIS_ANT_TX_TST_DLT = 0.1;                % delta   data of the test distances of the tx antenna in lamda
 DATA_DIS_ANT_TX_TST_MAX = 20;                 % maximum data of the test distances of the tx antenna in lamda

 DATA_ANG_TST_DLT        = 0.1;                % delta   data of test angles in degree
 DATA_ANG_TST_RNG        = 89;                 % maximum data of test angles in degree




%%*** DERIVED PARAMETER *********************************************************
DATA_ANG_TST        = -DATA_ANG_TST_RNG:DATA_ANG_TST_DLT:DATA_ANG_TST_RNG;
DATA_DIS_ANT_TX_TST = DATA_DIS_ANT_TX_TST_MIN:DATA_DIS_ANT_TX_TST_DLT:DATA_DIS_ANT_TX_TST_MAX;

NUMB_ANT            = numel(DATA_DIS_ANT_RX) * NUMB_ANT_TX;
NUMB_ANG_TST        = numel(DATA_ANG_TST);
NUMB_DIS_ANT_TX_TST = numel(DATA_DIS_ANT_TX_TST);




%*** MAIN BODY *****************************************************************
%% open figure
figure(INDX_FIG); INDX_FIG = INDX_FIG + 1;
if NUMB_DIS_ANT_TX_TST == 1
    set(gcf, 'position', [800, 300, 600, 400]);
else
    set(gcf, 'position', [800, 300, 600, 400]);
end

%% traverse
datPowLobSid = zeros(1, NUMB_DIS_ANT_TX_TST);
datWidLobMai = zeros(1, NUMB_DIS_ANT_TX_TST);
for idxDisAntTx = 1:NUMB_DIS_ANT_TX_TST
    %% prepare DATA_DIS_ANT
    DATA_DIS_ANT = [];
    for idxAntTx = 1:NUMB_ANT_TX
        DATA_DIS_ANT = [DATA_DIS_ANT, DATA_DIS_ANT_RX + idxAntTx * DATA_DIS_ANT_TX_TST(idxDisAntTx)];
    end
    DATA_DIS_ANT = DATA_DIS_ANT';

    %% prepare datCoe
    datCoeTst = zeros(NUMB_ANT, NUMB_ANG_TST);
    for idxAng = 1:NUMB_ANG_TST
        datAng = DATA_ANG_TST(idxAng);
        datCoe = exp(-1i * 2 * pi * DATA_DIS_ANT * sin(datAng / 180 * pi));
        datCoeTst(:, idxAng) = datCoe;
    end

    %% prepare datSig
    datSig = zeros(numel(DATA_DIS_ANT), 1) + 1;

    %% get curve
    % calculate
    datPowTst = zeros(1, NUMB_ANG_TST);
    for idxAng = 1:NUMB_ANG_TST
        datPowTmp = datCoeTst(:, idxAng) .* datSig;
        datPowTst(idxAng) = 20 * log10(abs(sum(datPowTmp)));
    end
    % plot
    if NUMB_DIS_ANT_TX_TST == 1
        plot(DATA_ANG_TST, datPowTst);
        hold on;
    end

    %% get peak
    % calculate
    [datPks, idxPks] = findpeaks(datPowTst);
    [datSrt, idxSrt] = sort(datPks, 'descend');
    idxAng = idxPks(idxSrt);
    idxAng = idxAng(1:min(3,numel(idxAng)));
    % plot
    if NUMB_DIS_ANT_TX_TST == 1
        for idxObj = 1:numel(idxAng)
            datAng = DATA_ANG_TST(idxAng(idxObj));
            datPow = datPowTst(idxAng(idxObj));
            plot(datAng, datPow, 'xk', 'MarkerSize', 10);
            text(datAng - 0.5, datPow + 1.5,                                            ...
                ['\downarrow', num2str(datAng,'%.2f'), ', ', num2str(datPow,'%.2f')]    ...
            );
        end
    end
    % save
    datPowLobSid(idxDisAntTx) = datSrt(2) - datSrt(1);

    %% get -3 db point
    datWidLobMai(idxDisAntTx) = inf;
    for idxAng = 2:NUMB_ANG_TST
        if (datPowTst(idxAng - 1) - (max(datPowTst) - 3)) * (datPowTst(idxAng) - (max(datPowTst) - 3)) < 0
            % calculate
            if datPowTst(idxAng - 1) > (max(datPowTst) - 3)
                datAng = DATA_ANG_TST(idxAng - 1);
                datPow = datPowTst (idxAng - 1);
            else
                datAng = DATA_ANG_TST(idxAng);
                datPow = datPowTst (idxAng);
            end
            % plot
            if NUMB_DIS_ANT_TX_TST == 1
                plot(datAng, datPow, 'xk', 'MarkerSize', 10);
                text(datAng - 0.5, datPow - 1.5,                                          ...
                    ['\uparrow', num2str(datAng,'%.2f'), ', ', num2str(datPow,'%.2f')]    ...
                );
            end
            % save
            if abs(datAng) < datWidLobMai(idxDisAntTx)
                datWidLobMai(idxDisAntTx) = abs(datAng);
            end
        end
    end
end

%% plot
if NUMB_DIS_ANT_TX_TST == 1
    % tune figure
    grid on;
    axis([min(DATA_ANG_TST), max(DATA_ANG_TST), max(datPowTst) - 20, max(datPowTst) + 5])
    title('dbf curve');
    xlabel('angle (degree)');
    ylabel('power (dB)');
    % save figure
    fig = getframe(gcf);
    img = frame2im(fig);
    imwrite(img, 'dump/curve.png');
else
    % plot
    %[ax] = plotyy(DATA_DIS_ANT_TX_TST, datWidLobMai, DATA_DIS_ANT_TX_TST, datPowLobSid);
    yyaxis left;
    plot(DATA_DIS_ANT_TX_TST, datWidLobMai);
    yyaxis right;
    plot(DATA_DIS_ANT_TX_TST, datPowLobSid);
    % tune figure
    grid on;
    strTitle1 = sprintf('TX(2): %.1f:%.1f:%.1f;  RX(4): %.1f',    ...
        DATA_DIS_ANT_TX_TST_MIN,                                  ...
        DATA_DIS_ANT_TX_TST_DLT,                                  ...
        DATA_DIS_ANT_TX_TST_MAX,                                  ...
        DATA_DIS_ANT_RX(1)                                        ...
    );
    strTitle2 = sprintf(', %.1f', DATA_DIS_ANT_RX(2:end));
    title([strTitle1,strTitle2]);
    xlabel('tx space (lamda)')
    yyaxis left;
    ylabel('main lobe width (degree)')
    yyaxis right;
    ylabel('side lobe power (db)')
    % save
    fig = getframe(gcf);
    img = frame2im(fig);
    imwrite(img, 'dump/mainLobeWidth_and_sideLobePower_vs_txDistance.png');
end




%*** INIT **********************************************************************
toc;
