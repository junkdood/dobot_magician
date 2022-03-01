#pragma once

#include "dobot/common.h"

using casadi::DM;
using casadi::MX;
using casadi::Function;
using casadi::Opti;
using casadi::Slice;

class DirectCollocationSolver{
public:
    DirectCollocationSolver(const arm_model& _model, const constraint_value& _constraint);
    ~DirectCollocationSolver(){ };
    bool setupProblemColloc(const Settings& _settings);
    bool solveColloc(const State& initialState, const State& finalState, const State& Kalman);
    void getSolutionColloc(DM& state, DM& control);
private:
    Function systemDynamics;
    Function getSystemDynamics();
    void setOptColloc();
    void setParametersValue(const State& initialState, const State& finalState, const State& Kalman);


    arm_model model;
    Settings settings;

    enum SolverState{NOT_INITIALIZED=0, PROBLEM_SET, PROBLEM_SOLVED};
    SolverState solverState;

    Opti opti;
    std::unique_ptr<casadi::OptiSol> solution;
    MX initialStateParameters, finalStateParameters, KalmanParameters;
    MX X, A, V, T;
    MX minJ0, maxJ0, minJ1, maxJ1, minJ2, maxJ2, minJ1subJ2, maxJ1subJ2, minV, maxV;
};

class KalmanFilter{
    public:
    KalmanFilter(double dt);
    ~KalmanFilter(){};


    private:
    DM A;
    DM B;
    DM H;
    DM x_evlt; //最终预测值
    DM x_pdct; //systemDynamics估计的值
    DM z_meas; //观测值(真值)
    DM pk; //估计与真值协方差
    DM pk_p; //预测与真值协方差
    DM k;
};