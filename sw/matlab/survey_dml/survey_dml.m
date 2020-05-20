%-------------------------------------------------------------------------------
  %
  %  Filename       : survey_dml.m
  %  Author         : Huang Leilei
  %  Created        : 2020-05-20
  %  Description    : survey_dml
  %
%-------------------------------------------------------------------------------

%*** INIT **********************************************************************
clc;
clear;




%*** PARAMETER *****************************************************************
NUM_TST = 1000;
NUM_SAM = 1;
DIS_ANT = 0.5 * (0:3);
%DIS_ANT = 0.5 * [0,1,4,6];
%DIS_ANT = 0.5 * (0:7);
%DIS_ANT = 0.5 * [[0,1,4,6],[0,1,4,6]+8];
ANG_OBJ = [0, 3];
SNR_OBJ = 20;
ANG_TST = -10:1:10;




%%*** DERIVED PARAMETER *********************************************************
NUM_ANT = numel(DIS_ANT);
NUM_OBJ = numel(ANG_OBJ);
NUM_ANG = numel(ANG_TST);




%*** MAIN BODY *****************************************************************
%% prepare P
P_array = ones(NUM_ANT, NUM_ANT, NUM_TST, NUM_TST);
for idx_0 = 1:NUM_ANG
    ang_0 = ANG_TST(idx_0);
    for idx_1 = (idx_0+1):NUM_ANG
        ang_1 = ANG_TST(idx_1);
        A = [exp(-1i * 2 * pi * DIS_ANT' * sin(ang_0 / 180 * pi))     ...
            ,exp(-1i * 2 * pi * DIS_ANT' * sin(ang_1 / 180 * pi))]    ...
        ;
        P = A * (A' * A)^-1 * A';
        P_array(:, :, idx_0, idx_1) = P;
    end
end

%% traverse
idx_bst = ones(NUM_TST, 2);
for idx_tst = 1:NUM_TST
    %% prepare X
    X = exp(-1i * 2 * pi * DIS_ANT' * sin(ANG_OBJ / 180 * pi)) * ones(NUM_OBJ, NUM_SAM);
    X = awgn(X, SNR_OBJ, 'measured');

    %% prepare R
    R = 0;
    for i = 1:NUM_SAM
        R = R + X(:, i) * X(:, i)';
    end
    R = R / NUM_SAM;

    %% get cost
    cst_bst = 0;
    for idx_0 = 1:NUM_ANG
        for idx_1 = idx_0+1:NUM_ANG
            Y = P_array(:, :, idx_0, idx_1) * R;
            cst_cur = abs(trace(Y));
            if cst_cur > cst_bst
                cst_bst = cst_cur;
                idx_bst(idx_tst, :) = [idx_0, idx_1];
            end
        end
    end
end

%% plot
for idx_ang = 1:2
    figure(idx_ang);
    ang_bst = ANG_TST(idx_bst(:, idx_ang));
    histogram(ang_bst);
    err = sum((ang_bst - ANG_OBJ(idx_ang)) .^ 2) / NUM_TST;
    fprintf('averaged SSE of angle %d is %.2f\n', idx_ang, err);
    grid on;
end
