%-------------------------------------------------------------------------------
  %
  %  Filename       : cstCustom.m
  %  Author         : Huang Leilei
  %  Created        : 2020-07-19
  %  Description    : survey_dbscan
  %
%-------------------------------------------------------------------------------
  %
  %  Modified       : 2020-08-01 by HLL
  %  Description    : actual unit adopted
  %
%-------------------------------------------------------------------------------

function cost = cstCustom(dat1, dat2)

% IDX_RNG = 1;
% IDX_VEL = 2;
% IDX_ANG = 3;
% IDX_SNR = 4;

cost = 0;
scale = [0.3^-1, 0.14^-1, 1 / pi * 180 * 0.5^-1, 1];
for i = [1, 2, 3]
    cost = cost + (dat1(:,i) - dat2(:,i)) .^ 2 * scale(i) ^ 2;
end
cost = cost .^ 0.5;
