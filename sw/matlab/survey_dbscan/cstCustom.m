%-------------------------------------------------------------------------------
  %
  %  Filename       : cstCustom.m
  %  Author         : Huang Leilei
  %  Created        : 2020-07-19
  %  Description    : survey_dbscan
  %
%-------------------------------------------------------------------------------

function cost = cstCustom(dat1, dat2)

% IDX_RNG = 1;
% IDX_VEL = 2;
% IDX_ANG = 3;
% IDX_SNR = 4;

cost = 0;
scale = [1, 1, 1 / pi * 180, 1];
for i = [1, 2, 3]
    cost = cost + (dat1(:,i) - dat2(:,i)) .^ 2 * scale(i) ^ 2;
end
cost = cost .^ 0.5;
